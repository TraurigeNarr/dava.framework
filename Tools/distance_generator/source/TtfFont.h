#ifndef __DISTANCE_GENERATOR__TTFFONT__
#define __DISTANCE_GENERATOR__TTFFONT__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "Singleton.h"

#include <string>

class FtLibrary: public Singleton<FtLibrary>
{
public:
    bool IsInit();
    FT_Library& GetLibrary();

    FtLibrary();

private:
    bool libraryInit;
    FT_Library library;

    ~FtLibrary();
};

class TtfFont
{
public:
    TtfFont();
    ~TtfFont();

    bool Init(const std::string& file);
    void DeInit();

    void SetSize(int size);
    int GetSize();

    bool SetCharMap(int charmap);
    int GetCharMap();

    float GetLineHeight();
	float GetBaseline();

    FT_Face& GetFace();
protected:
    int size;
    FT_Face face;
    bool faceInit;
    std::string fileName;
};

#endif /* defined(__DISTANCE_GENERATOR__TTFFONT__) */
