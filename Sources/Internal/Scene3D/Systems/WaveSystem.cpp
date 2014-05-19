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


#include "WaveSystem.h"
#include "Scene3D/Entity.h"
#include "Scene3D/Components/ComponentHelpers.h"
#include "Scene3D/Components/WaveComponent.h"
#include "Scene3D/Components/TransformComponent.h"
#include "Scene3D/Systems/EventSystem.h"
#include "Scene3D/Scene.h"
#include "Utils/Random.h"
#include "Math/Math2D.h"

namespace DAVA
{

WaveSystem::WaveInfo::WaveInfo(WaveComponent * _component) :
component(_component),
currentWaveRadius(0.f)
{
    if(component->GetDampingRatio() < EPSILON)
        maxRadius = component->GetInfluenceRadius();
    else
        maxRadius = Min(component->GetInfluenceRadius(), 1.f/component->GetDampingRatio());

    maxRadiusSq = maxRadius * maxRadius;

    center = GetTransformComponent(component->GetEntity())->GetWorldTransform().GetTranslationVector();
}

WaveSystem::WaveSystem(Scene * scene) : 
    SceneSystem(scene)
{
    RenderOptions * options = RenderManager::Instance()->GetOptions();
    options->AddObserver(this);
    HandleEvent(options);
}

WaveSystem::~WaveSystem()
{
    ClearWaves();
}

void WaveSystem::WaveTriggered(WaveComponent * component)
{
    if(!isWavesEnabled)
        return;

    waves.push_back(new WaveInfo(component));
}

void WaveSystem::Process(float32 timeElapsed)
{
    Vector<WaveInfo *>::iterator it = waves.begin();
    while(it != waves.end())
    {
        WaveInfo * info = *it;
        info->currentWaveRadius += info->component->GetWaveSpeed() * timeElapsed;

        if(info->currentWaveRadius >= info->maxRadius)
        {
            SafeDelete(info);
            it = waves.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

Vector3 WaveSystem::GetWaveDisturbance(const Vector3 & inPosition) const
{
    Vector3 ret;
    int32 wavesCount = waves.size();
    for(int32 i = 0; i < wavesCount; ++i)
    {
        WaveInfo * info = waves[i];
        Vector3 direction = inPosition - info->center;
        float32 distanceSq = direction.SquareLength();
        if(distanceSq > EPSILON && distanceSq < info->maxRadiusSq)
        {
            WaveComponent * component = info->component;

            float32 damping = 1 - component->GetDampingRatio() * info->currentWaveRadius; //damping function: D = 1 - k * x

            DVASSERT(damping >= 0.f);

            float32 distance = direction.Normalize();
            float32 dt = abs(info->currentWaveRadius - distance);
            float32 value = Max(1 - dt / component->GetWaveLenght(), 0.f) * component->GetWaveAmplitude() * component->GetWaveSpeed() * damping; // wave function: A = (1 - x/L) * A0

            DVASSERT(value >= 0.f);

            ret += direction * value;
        }
    }

    return ret;
}

void WaveSystem::ClearWaves()
{
    int32 wavesCount = waves.size();
    for(int32 i = 0; i < wavesCount; ++i)
        SafeDelete(waves[i]);

    waves.clear();
}

void WaveSystem::HandleEvent(Observable * observable)
{
    RenderOptions * options = static_cast<RenderOptions *>(observable);
    isWavesEnabled = options->IsOptionEnabled(RenderOptions::WAVE_DISTURBANCE_PROCESS);

    if(!isWavesEnabled)
        ClearWaves();
}

};