/*==================================================================================
    Copyright (c) 2008, binaryzebra
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the binaryzebra nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=====================================================================================*/


#include "Platform/Thread.h"

#ifndef __DAVAENGINE_WIN32__
#include <time.h>
#include <errno.h>
#endif

#include "Thread/LockGuard.h"

namespace DAVA
{

Set<Thread *> Thread::threadList;
Mutex Thread::threadListMutex;
Map<Thread::NativeId, Thread::Id> Thread::threadIdList;
Mutex Thread::threadIdListMutex;

Thread::Id Thread::mainThreadId = 0;
Thread::Id Thread::glThreadId = 0;

ConditionalVariable::ConditionalVariable()
{
    int32 ret = pthread_cond_init(&cv, 0);
    if (0 != ret)
    {
        Logger::FrameworkDebug("[ConditionalVariable::ConditionalVariable()]: pthread_cond_init error code %d", ret);
    }
}

ConditionalVariable::~ConditionalVariable()
{
    int32 ret = pthread_cond_destroy(&cv);
    if (0 != ret)
    {
        Logger::FrameworkDebug("[ConditionalVariable::~ConditionalVariable()]: pthread_cond_destroy error code %d", ret);
    }
}

void Thread::InitMainThread()
{
    mainThreadId = GetCurrentId();
}

void Thread::InitGLThread()
{
	glThreadId = GetCurrentId();
}

bool Thread::IsMainThread()
{
    if (0 == mainThreadId)
    {
        Logger::Error("Main thread not initialized");
    }

    //Not an any thread which calls IsMainThread is DAVA::Thread, so it sould not contain nativeId
    Id currentId = GetCurrentId();

    return currentId == mainThreadId || currentId == glThreadId;
}

Thread::Id Thread::GetCurrentId()
{
    // try to find in map
    // if not found - create new id
    Id retId;
    
    LockGuard<Mutex> locker(threadIdListMutex);
    NativeId threadNativeIdentifier = GetCurrentNativeId();
    Map<NativeId, Id>::iterator it = threadIdList.find(threadNativeIdentifier);
    if (it == threadIdList.end())
    {
        static Id newId = 1;
        retId = newId;
        threadIdList[threadNativeIdentifier] = newId;
        ++newId;
    }
    else
    {
        retId = it->second;
    }

    return retId;
}

Thread::Id Thread::GetMainId()
{
    return mainThreadId;
}

Thread * Thread::Create(const Message& msg)
{
	Thread * t = new Thread(msg);
    t->state = STATE_CREATED;

    return t;
}

void Thread::Kill()
{
    // it is possible to kill thread just after creating or starting and the problem is - thred changes state
    // to STATE_RUNNING insite threaded function - so that could not happens in that case. Need some time.
    DVASSERT(STATE_CREATED != state);

    // Important - DO NOT try to wait RUNNING state because that state wll not appear if thread is not started!!!
    // You can wait RUNNING state, but not from thred which should call Start() for created Thread.

    if (STATE_RUNNING == state)
    {
        KillNative();
        state = STATE_KILLED;
        threadIdListMutex.Lock();
        threadIdList.erase(nativeId);
        threadIdListMutex.Unlock();
    }
}

void Thread::KillAll()
{
    LockGuard<Mutex> locker(threadListMutex);
    Set<Thread *>::iterator end = threadList.end();
    for (Set<Thread *>::iterator i = threadList.begin(); i != end; ++i)
    {
        (*i)->Kill();
    }
}

void Thread::Cancel()
{
    // it is possible to cancel thread just after creating or starting and the problem is - thred changes state
    // to STATE_RUNNING insite threaded function - so that could not happens in that case. Need some time.
    DVASSERT(STATE_CREATED != state);

    // Important - DO NOT try to wait RUNNING state because that state wll not appear if thread is not started!!!
    // You can wait RUNNING state, but not from thred which should call Start() for created Thread.
    if (STATE_RUNNING == state)
    {
        state = STATE_CANCELLING;
    }
}

void Thread::CancelAll()
{
	LockGuard<Mutex> locker(threadListMutex);
    Set<Thread *>::iterator end = threadList.end();
    for (Set<Thread *>::iterator i = threadList.begin(); i != end; ++i)
    {
        (*i)->Cancel();
    }
}


Thread::Thread(const Message& _msg)
    : BaseObject()
    , msg(_msg)
    , state(STATE_CREATED)
    , id(0)
	, nativeId(0)
{
    threadListMutex.Lock();
    threadList.insert(this);
    threadListMutex.Unlock();

    Init();
}

Thread::~Thread()
{
    Shutdown();
    threadIdListMutex.Lock();
    threadIdList.erase(nativeId);
    threadIdListMutex.Unlock();
    threadListMutex.Lock();
    threadList.erase(this);
    threadListMutex.Unlock();
}

void Thread::Wait(ConditionalVariable * cv, Mutex * mutex)
{
    int32 ret = 0;

    if ((ret = pthread_cond_wait(&cv->cv, (pthread_mutex_t*)(&mutex->mutex))))
    {
        Logger::FrameworkDebug("[Thread::Wait]: pthread_cond_wait error code %d", ret);
    }
}

void Thread::Signal(ConditionalVariable * cv)
{
    int32 ret = pthread_cond_signal(&cv->cv);
    if (ret)
    {
        Logger::FrameworkDebug("[Thread::Signal]: pthread_cond_signal error code %d", ret);
    }
}
    
void Thread::Broadcast(ConditionalVariable * cv)
{
    int32 ret = pthread_cond_broadcast(&cv->cv);
    if (ret)
    {
        Logger::FrameworkDebug("[Thread::Broadcast]: pthread_cond_broadcast error code %d", ret);
    }
}
    
void Thread::ThreadFunction(void *param)
{
    Thread * t = (Thread *)param;
    t->id = GetCurrentId();
    t->nativeId = GetCurrentNativeId();

    if (STATE_CREATED == t->state)
    {
        t->state = STATE_RUNNING;
        t->msg(t);
    }

    switch(t->state)
    {
    case STATE_CANCELLING:
        t->state = STATE_CANCELLED;
        break;
    case STATE_RUNNING:
        t->state = STATE_ENDED;
        break;
    default:
        break;
    }

    // thread is finishing so we need to unregister it
    threadIdListMutex.Lock();
    threadIdList.erase(t->nativeId);
    threadIdListMutex.Unlock();

    t->Release();
}
    
};
