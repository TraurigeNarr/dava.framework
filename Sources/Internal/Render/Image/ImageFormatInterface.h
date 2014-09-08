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



#ifndef __DAVAENGINE_IMAGE_FORMAT_INTERFACE_H__
#define __DAVAENGINE_IMAGE_FORMAT_INTERFACE_H__

#include "Base/BaseTypes.h"
#include "Render/RenderBase.h"
#include "FileSystem/FilePath.h"

namespace DAVA 
{

class File;
class Image;

class ImageFormatInterface
{
public:
    
    virtual ~ImageFormatInterface()
    {};
    
    virtual bool IsImage(File *file) const = 0;
    
    virtual eErrorCode ReadFile(File *infile, Vector<Image *> &imageSet, int32 fromMipmap) const = 0;
    
    virtual eErrorCode WriteFile(const FilePath & fileName, const Vector<Image *> &imageSet, PixelFormat compressionFormat) const = 0;
    virtual eErrorCode WriteFileAsCubeMap(const FilePath & fileName, const Vector<Image *> &imageSet, PixelFormat compressionFormat) const = 0;
    
    virtual uint32 GetDataSize(File *infile) const = 0;

	Size2i GetImageSize(const FilePath & fileName) const;
	virtual Size2i GetImageSize(File *infile) const = 0;

    
    inline bool IsFileExtensionSupported(const String& extension) const;
    
protected:

    Vector<String> supportedExtensions;
    
};
    
inline bool ImageFormatInterface::IsFileExtensionSupported(const String& extension) const
{
    return std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end();
}

};

#endif // __DAVAENGINE_IMAGE_FORMAT_INTERFACE_H__