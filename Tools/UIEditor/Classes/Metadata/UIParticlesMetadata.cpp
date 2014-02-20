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

#include "UIParticlesMetadata.h"

UIParticlesMetadata::UIParticlesMetadata(QObject* parent) :
    UIControlMetadata(parent)
{
}

void UIParticlesMetadata::InitializeControl(const String& controlName, const Vector2& position)
{
	UIControlMetadata::InitializeControl(controlName, position);
}

UIParticles* UIParticlesMetadata::GetActiveUIParticles() const
{
    return dynamic_cast<UIParticles*>(GetActiveUIControl());
}

void UIParticlesMetadata::Start()
{
    if (!VerifyActiveParamID())
    {
        return;
    }

    GetActiveUIParticles()->Start();
}

void UIParticlesMetadata::Stop()
{
    if (!VerifyActiveParamID())
    {
        return;
    }
    
    GetActiveUIParticles()->Stop();
}

void UIParticlesMetadata::Reload()
{
    if (!VerifyActiveParamID())
    {
        return;
    }
    
    GetActiveUIParticles()->Reload();
}

QString UIParticlesMetadata::GetEffectPath() const
{
    if (!VerifyActiveParamID())
    {
        return QString();
    }
    
    return QString::fromStdString(GetActiveUIParticles()->GetEffectPath().GetAbsolutePathname());
}

void UIParticlesMetadata::SetEffectPath(const QString& value)
{
    if (!VerifyActiveParamID())
    {
        return;
    }
    
	GetActiveUIParticles()->Load(value.toStdString());
}

bool UIParticlesMetadata::GetAutostart() const
{
    if (!VerifyActiveParamID())
    {
        return false;
    }
    
    return GetActiveUIParticles()->IsAutostart();
}

void UIParticlesMetadata::SetAutostart(bool value)
{
    if (!VerifyActiveParamID())
    {
        return;
    }
    
    GetActiveUIParticles()->SetAutostart(value);
}
