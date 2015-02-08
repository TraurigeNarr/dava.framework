
#ifndef __DAVAENGINE_MEMPROFSERVICE_H__
#define __DAVAENGINE_MEMPROFSERVICE_H__

#include "Base/BaseTypes.h"
#include "Network/NetService.h"

#include "memprof/mem_profiler_types.h"

namespace DAVA
{
namespace Net
{

class MemProfService : public NetService
{    
public:
    MemProfService();
    virtual ~MemProfService();

    void OnUpdate(float32 timeElapsed);

    void ChannelOpen() override;
    void ChannelClosed(const char8* message) override;
    void PacketReceived(const void* packet, size_t length) override;
    void PacketSent() override;
    void PacketDelivered() override;
    
private:
    void SendNextRecord();
    
    bool Enqueue(net_mem_stat_t* h);
    net_mem_stat_t* GetFirstMessage();
    void RemoveFirstMessage();
    
private:
    uint32 period;
    uint32 passed;
    size_t maxQueueSize;
    uint32 timestamp;
    Deque<net_mem_stat_t*> queue;
};

}   // namespace Net
}   // namespace DAVA

#endif // __DAVAENGINE_MEMPROFSERVICE_H__
