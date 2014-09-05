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


#ifndef __DAVAENGINE_SCENE3D_RENDERLAYER_H__
#define	__DAVAENGINE_SCENE3D_RENDERLAYER_H__

#include "Base/BaseTypes.h"
#include "Base/FastName.h"
#include "Render/Highlevel/RenderBatch.h"
#include "Render/Highlevel/RenderFastNames.h"

namespace DAVA
{

class RenderLayerBatchArray;
class Camera;
class OcclusionQuery;
    
class RenderLayer
{
public:
    RenderLayer(const FastName & name, uint32 sortingFlags, RenderLayerID id);
    virtual ~RenderLayer();
    
    inline RenderLayerID GetRenderLayerID() const;
	inline const FastName & GetName() const;
	inline uint32 GetFlags() const;

    virtual void Draw(const FastName & ownerRenderPass, Camera * camera, RenderLayerBatchArray * renderLayerBatchArray);
    
    inline uint32 GetFragmentStats() const;
    
protected:
    FastName name;
    uint32 flags;
    RenderLayerID id;
    
    OcclusionQuery* occlusionQuery;
    uint32 lastFragmentsRenderedValue;
    bool queryPending;
    
public:
    INTROSPECTION(RenderLayer,
        MEMBER(name, "Name", I_VIEW )
        //COLLECTION(renderBatchArray, "Render Batch Array", I_VIEW)
    );
};
    
inline RenderLayerID RenderLayer::GetRenderLayerID() const
{
    return id;
}
    
inline const FastName & RenderLayer::GetName() const
{
    return name;
}

inline uint32 RenderLayer::GetFlags() const
{
    return flags;
}

inline uint32 RenderLayer::GetFragmentStats() const
{
    return lastFragmentsRenderedValue;
}
    
} // ns

#endif	/* __DAVAENGINE_SCENE3D_RENDERLAYER_H__ */
