/*==================================================================================
    Copyright(c) 2008, binaryzebra
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

#ifndef __DAVAENGINE_TCPACCEPTORTEMPLATE_H__
#define __DAVAENGINE_TCPACCEPTORTEMPLATE_H__

#include <Base/Noncopyable.h>
#include <Debug/DVAssert.h>

#include "IOLoop.h"
#include "Endpoint.h"

namespace DAVA
{
namespace Net
{

/*
 Template class TCPAcceptorTemplate wraps TCP acceptor from underlying network library and provides interface to user
 through CRTP idiom. Class specified by template parameter T should inherit TCPAcceptorTemplate and provide some
 members that will be called by base class (TCPAcceptorTemplate) using compile-time polymorphism.
*/
template <typename T>
class TCPAcceptorTemplate : private Noncopyable
{
public:
    TCPAcceptorTemplate(IOLoop* ioLoop);
    ~TCPAcceptorTemplate();

    int32 Bind(const Endpoint& endpoint);

protected:
    bool IsOpen() const;
    void DoOpen();
    int32 DoAccept(uv_tcp_t* client);

    int32 DoStartListen(int32 backlog);
    void DoClose();

private:
    // Thunks between C callbacks and C++ class methods
    static void HandleCloseThunk(uv_handle_t* handle);
    static void HandleConnectThunk(uv_stream_t* handle, int error);

private:
    uv_tcp_t uvhandle;          // libuv handle itself
    IOLoop* loop;               // IOLoop object handle is attached to
    bool isOpen;                // Handle has been initialized and can be used in operations
    bool isClosing;             // Close has been issued and waiting for close operation complete, used mainly for asserts
};

//////////////////////////////////////////////////////////////////////////
template <typename T>
TCPAcceptorTemplate<T>::TCPAcceptorTemplate(IOLoop* ioLoop) : uvhandle()
                                                    , loop(ioLoop)
                                                    , isOpen(false)
                                                    , isClosing(false)
{
    DVASSERT(ioLoop != NULL);
}

template <typename T>
TCPAcceptorTemplate<T>::~TCPAcceptorTemplate()
{
    // libuv handle should be closed before destroying object
    DVASSERT(false == isOpen && false == isClosing);
}

template <typename T>
int32 TCPAcceptorTemplate<T>::Bind(const Endpoint& endpoint)
{
    DVASSERT(false == isClosing);
    if (!isOpen)
        DoOpen();
    return uv_tcp_bind(&uvhandle, endpoint.CastToSockaddr(), 0);
}

template <typename T>
bool TCPAcceptorTemplate<T>::IsOpen() const
{
    return isOpen;
}

template <typename T>
void TCPAcceptorTemplate<T>::DoOpen()
{
    DVASSERT(false == isOpen && false == isClosing);
    uv_tcp_init(loop->Handle(), &uvhandle);
    isOpen = true;
    uvhandle.data = this;
}

template <typename T>
int32 TCPAcceptorTemplate<T>::DoAccept(uv_tcp_t* client)
{
    DVASSERT(true == isOpen && false == isClosing && client != NULL);
    return uv_accept(reinterpret_cast<uv_stream_t*>(&uvhandle), reinterpret_cast<uv_stream_t*>(client));
}

template <typename T>
int32 TCPAcceptorTemplate<T>::DoStartListen(int32 backlog)
{
    // Acceptor should be bound first
    DVASSERT(true == isOpen && false == isClosing && backlog > 0);
    return uv_listen(reinterpret_cast<uv_stream_t*>(&uvhandle), backlog, &HandleConnectThunk);
}

template <typename T>
void TCPAcceptorTemplate<T>::DoClose()
{
    DVASSERT(true == isOpen && false == isClosing);
    if (true == isOpen)
    {
        isOpen = false;
        isClosing = true;
        uv_close(reinterpret_cast<uv_handle_t*>(&uvhandle), &HandleCloseThunk);
    }
}

///   Thunks   ///////////////////////////////////////////////////////////
template <typename T>
void TCPAcceptorTemplate<T>::HandleCloseThunk(uv_handle_t* handle)
{
    TCPAcceptorTemplate* self = static_cast<TCPAcceptorTemplate*>(handle->data);
    self->isClosing = false;    // Mark acceptor has been closed and clear handle
    Memset(&self->uvhandle, 0, sizeof(self->uvhandle));

    static_cast<T*>(self)->HandleClose();
}

template <typename T>
void TCPAcceptorTemplate<T>::HandleConnectThunk(uv_stream_t* handle, int error)
{
    TCPAcceptorTemplate* self = static_cast<TCPAcceptorTemplate*>(handle->data);
    static_cast<T*>(self)->HandleConnect(error);
}

}   // namespace Net
}	// namespace DAVA

#endif  // __DAVAENGINE_TCPACCEPTORTEMPLATE_H__