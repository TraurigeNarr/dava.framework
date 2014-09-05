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


#ifndef __DAVAENGINE_SCENE3D_RENDER_PASS_H__
#define	__DAVAENGINE_SCENE3D_RENDER_PASS_H__

#include "Base/BaseTypes.h"
#include "Base/FastName.h"
#include "Render/Highlevel/RenderLayer.h"
#include "Render/Highlevel/RenderFastNames.h"

namespace DAVA
{
class RenderPassBatchArray;
class Camera;

class RenderPass
{
public:    
    RenderPass(const FastName & name, RenderPassID id);
    virtual ~RenderPass();
    
    void InitDefaultLayers();
    
    inline RenderPassID GetRenderPassID() const;
    inline const FastName & GetName() const;
    
	void AddRenderLayer(RenderLayer * layer, const FastName & afterLayer);
	void RemoveRenderLayer(RenderLayer * layer);

    virtual void Draw(RenderSystem * renderSystem, uint32 clearBuffers);
    
    inline uint32 GetRenderLayerCount() const;
    inline RenderLayer * GetRenderLayer(uint32 index) const;
    
    
protected:
    // TODO: add StaticVector container
    Vector<RenderLayer*> renderLayers;    
    FastName name;
    RenderPassID id;

    /*convinience*/
    void PrepareVisibilityArrays(Camera *camera, RenderSystem * renderSystem);
    void DrawLayers(Camera *camera);
    void ClearBuffers(uint32 clearBuffers);

	RenderPassBatchArray * renderPassBatchArray;
	VisibilityArray visibilityArray;
public:
    
    INTROSPECTION(RenderPass,
        COLLECTION(renderLayers, "Render Layers", I_VIEW | I_EDIT)
        MEMBER(name, "Name", I_VIEW)
    );

	friend class RenderSystem;
};

inline RenderPassID RenderPass::GetRenderPassID() const
{
    return id;
}

inline const FastName & RenderPass::GetName() const
{
    return name;
}

inline uint32 RenderPass::GetRenderLayerCount() const
{
    return (uint32)renderLayers.size();
}
    
inline RenderLayer * RenderPass::GetRenderLayer(uint32 index) const
{
    return renderLayers[index];
}

class WaterPrePass : public RenderPass
{    
public:
    inline void SetWaterLevel(float32 level){waterLevel = level;}
    WaterPrePass(const FastName & name, RenderPassID id);
    ~WaterPrePass();
protected:
    Camera *passMainCamera, *passDrawCamera;
    float32 waterLevel;
};
class WaterReflectionRenderPass  : public WaterPrePass
{        
public:    
    WaterReflectionRenderPass(const FastName & name, RenderPassID id);
	virtual void Draw(RenderSystem * renderSystem, uint32 clearBuffers);	
private:
    void UpdateCamera(Camera *camera);
};

class WaterRefractionRenderPass  : public WaterPrePass
{       
public:
    WaterRefractionRenderPass(const FastName & name, RenderPassID id);
    virtual void Draw(RenderSystem * renderSystem, uint32 clearBuffers);

};

class MainForwardRenderPass : public RenderPass
{	

public:
    MainForwardRenderPass(const FastName & name, RenderPassID id);
	~MainForwardRenderPass();
	virtual void Draw(RenderSystem * renderSystem, uint32 clearBuffers);

private:
	WaterReflectionRenderPass *reflectionPass;
    WaterRefractionRenderPass *refractionPass;
    Texture *reflectionTexture, *refractionTexture;    
    
    bool needWaterPrepass;
    AABBox3 waterBox;

    void PrepareReflectionRefractionTextures(RenderSystem * renderSystem);
};

} // ns

#endif	/* __DAVAENGINE_SCENE3D_RENDERLAYER_H__ */
