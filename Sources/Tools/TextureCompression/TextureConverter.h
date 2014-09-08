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


#ifndef __DAVAENGINE_TOOLS_TEXTURE_CONVERTER_H__
#define __DAVAENGINE_TOOLS_TEXTURE_CONVERTER_H__

#include "Base/BaseTypes.h"
#include "Render/RenderBase.h"
#include "FileSystem/FilePath.h"

namespace DAVA
{
	class TextureDescriptor;
	class TextureConverter
	{
	public:

        enum eConvertQuality
        {
            ECQ_FASTEST        =   0,
            ECQ_FAST,
            ECQ_NORMAL,
            ECQ_HIGH,
            ECQ_VERY_HIGH,
            
            ECQ_COUNT,
            ECQ_DEFAULT = ECQ_VERY_HIGH
        };

		
		static FilePath ConvertTexture(const TextureDescriptor &descriptor, eGPUFamily gpuFamily, bool updateAfterConversion, eConvertQuality quality);
		static bool CleanupOldTextures(const DAVA::TextureDescriptor *descriptor, const DAVA::eGPUFamily forGPU, const DAVA::PixelFormat format);
		static FilePath GetOutputPath(const TextureDescriptor &descriptor, eGPUFamily gpuFamily);
		
		DAVA_DEPRECATED(static void DeleteOldPVRTextureIfPowerVr_IOS(const DAVA::TextureDescriptor *descriptor, const DAVA::eGPUFamily gpu));
		DAVA_DEPRECATED(static void DeleteOldDXTTextureIfTegra(const DAVA::TextureDescriptor *descriptor, const DAVA::eGPUFamily gpu));
	};
}

#endif /* defined(__DAVAENGINE_TEXTURE_CONVERTER_H__) */