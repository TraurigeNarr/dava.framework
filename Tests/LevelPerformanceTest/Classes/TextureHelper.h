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


#ifndef __LEVELPERFORMANCETESTIPHONE__TEXTUREHELPER__
#define __LEVELPERFORMANCETESTIPHONE__TEXTUREHELPER__

#include "DAVAEngine.h"

using DAVA::String;
using DAVA::Map;
using DAVA::Texture;
using DAVA::Landscape;
using DAVA::FilePath;

class TextureHelper
{
public:
	static DAVA::uint32 GetSceneTextureMemory(DAVA::Scene* scene);
	static DAVA::uint32 GetSceneTextureFilesSize(DAVA::Scene* scene);

private:
	static DAVA::uint32 EnumerateSceneTextures(DAVA::Scene* scene);
	static DAVA::uint32 EnumerateSceneTexturesFileSize(DAVA::Scene* scene);

	static void EnumerateTextures(DAVA::Entity *forNode, Map<String, Texture *> &textures);
	static void CollectLandscapeTextures(DAVA::Map<DAVA::String, DAVA::Texture *> &textures, Landscape *forNode);
	static void CollectTexture(Map<String, Texture *> &textures, const FilePath &name, Texture *tex);
};

#endif /* defined(__LEVELPERFORMANCETESTIPHONE__TEXTUREHELPER__) */