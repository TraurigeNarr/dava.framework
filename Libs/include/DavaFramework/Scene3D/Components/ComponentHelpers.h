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



#ifndef __DAVAENGINE_COMPONENT_HELPERS_H__
#define __DAVAENGINE_COMPONENT_HELPERS_H__

#include "Base/BaseTypes.h"

namespace DAVA
{

class ParticleEmitter;
class ParticleEffectComponent;
class Entity;
class RenderObject;
class Light;
class LightComponent;
class Landscape;
class Camera;
class LodComponent;
class SoundComponent;
class SoundEvent;
class SkyboxRenderObject;
class SwitchComponent;
class QualitySettingsComponent;
class TransformComponent;
class RenderComponent;
class VegetationRenderObject;
class CustomPropertiesComponent;
class KeyedArchive;
class SpeedTreeComponent;
class WindComponent;
class WaveComponent;
class SpeedTreeObject;

ParticleEffectComponent * GetEffectComponent(Entity * fromEntity);
TransformComponent * GetTransformComponent(Entity * fromEntity);
RenderComponent * GetRenderComponent(const Entity *fromEntity);
RenderObject * GetRenderObject(const Entity * fromEntity);
SkyboxRenderObject * GetSkybox(const Entity * fromEntity);
VegetationRenderObject * GetVegetation(const Entity * fromEntity);
SpeedTreeObject * GetSpeedTreeObject(const Entity * fromEntity);

Light *GetLight(Entity * fromEntity);
LightComponent *GetLightComponent(Entity * fromEntity);
Landscape *GetLandscape(Entity * fromEntity);

Camera * GetCamera(Entity * fromEntity);

SoundComponent * GetSoundComponent(Entity * fromEntity);

LodComponent * GetLodComponent(Entity *fromEntity);
SwitchComponent* GetSwitchComponent(Entity *fromEntity);
    
uint32 GetLodLayersCount(Entity *fromEntity);
uint32 GetLodLayersCount(LodComponent *fromComponent);
    
    
void RecursiveProcessMeshNode(Entity * curr, void * userData, void(*process)(Entity*, void *));
void RecursiveProcessLodNode(Entity * curr, int32 lod, void * userData, void(*process)(Entity*, void*));

Entity * FindLandscapeEntity(Entity * rootEntity);
Landscape * FindLandscape(Entity * rootEntity);
Entity * FindVegetationEntity(Entity * rootEntity);
VegetationRenderObject* FindVegetation(Entity * rootEntity);

SpeedTreeComponent * GetSpeedTreeComponent(Entity * fromEntity);
WindComponent * GetWindComponent(Entity * fromEntity);
WaveComponent * GetWaveComponent(Entity * fromEntity);

QualitySettingsComponent * GetQualitySettingsComponent(const Entity *fromEntity);
    
CustomPropertiesComponent * GetCustomProperties(const Entity *fromEntity);
CustomPropertiesComponent * GetOrCreateCustomProperties(Entity *fromEntity);
KeyedArchive * GetCustomPropertiesArchieve(const Entity *fromEntity);

}

#endif //__DAVAENGINE_COMPONENT_HELPERS_H__