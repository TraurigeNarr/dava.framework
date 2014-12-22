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

#include <Utils/UTF8Utils.h>
#include <Thread/LockGuard.h>

#include <Network/Base/Endpoint.h>

#include "NetLogger.h"

namespace DAVA
{
namespace Net
{

NetLogger::NetLogger(bool selfInstallFlag, size_t queueSize)
    : selfInstall(selfInstallFlag)
    , maxQueueSize(queueSize > 1 ? queueSize : 100)
{
    if (selfInstall)
        Logger::AddCustomOutput(this);
}

NetLogger::~NetLogger()
{
    if (selfInstall)
        Logger::RemoveCustomOutput(this);
}

void NetLogger::ChannelOpen()
{
    SendNextRecord();   // start sending log records if any
}

void NetLogger::OnPacketSent(IChannel* channel, const void* buffer, size_t length)
{
    // Record has been sent and buffer can be deleted
    delete [] static_cast<const char8*>(buffer);
}

void NetLogger::OnPacketDelivered(IChannel* channel, uint32 packetId)
{
    // Record has been got by other side so we can remove that record
    RemoveFirstMessage();
    SendNextRecord();
}

void NetLogger::Output(Logger::eLogLevel ll, const char8* text)
{
    if(text)
        DoOutput(ll, text);
}

void NetLogger::Output(Logger::eLogLevel ll, const char16* text)
{
    if(text)
        Output(ll, UTF8Utils::EncodeToUTF8(WideString(text)).c_str());
}

void NetLogger::DoOutput(Logger::eLogLevel ll, const char8* text)
{
    // if queue has been previously empty then start sending
    if (EnqueueMessage(ll, text))
        SendNextRecord();
}

void NetLogger::SendNextRecord()
{
    LogRecord record;
    if (IsChannelOpen() && true == GetFirstMessage(record))
    {
        String timeStr = UTF8Utils::EncodeToUTF8(record.timestamp.AsWString(L"%Y-%m-%d %H:%M:%S"));
        const char* levelStr = Logger::Instance()->GetLogLevelString(record.level);

        size_t n = timeStr.size() + 1 + strlen(levelStr) + 1 + record.message.size();
        char8* buf = new char8[n + 1];  // this will be deleted in OnChannelSendComplete callback
        Snprintf(buf, n + 1, "%s %s %s", timeStr.c_str(), levelStr, record.message.c_str());
        Send(buf, n - 1, NULL);   // remove trailing '\n'
    }
}

bool NetLogger::EnqueueMessage(Logger::eLogLevel ll, const char8* message)
{
    LockGuard<Mutex> lock(mutex);
    bool wasEmpty = recordQueue.empty();
    if (maxQueueSize <= recordQueue.size())
        recordQueue.pop_front();
    recordQueue.push_back(LogRecord(DateTime::Now(), ll, message));
    return wasEmpty;
}

bool NetLogger::GetFirstMessage(LogRecord& record)
{
    LockGuard<Mutex> lock(mutex);
    if(!recordQueue.empty())
    {
        record = recordQueue.front();
        return true;
    }
    return false;
}

void NetLogger::RemoveFirstMessage()
{
    LockGuard<Mutex> lock(mutex);
    if(!recordQueue.empty())
    {
        recordQueue.pop_front();
    }
}

}   // namespace Net
}