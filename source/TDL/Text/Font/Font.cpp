
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include "TDL/Text/Font/Font.hpp"
#include <iostream>


namespace tdl {
    Font::Font() : _size(12){
        if (FT_Init_FreeType(&_library)) {
            std::cerr << "Error: Could not init freetype library" << std::endl;
        }
    }

    Font::~Font() {
        FT_Done_Face(_face);
        FT_Done_FreeType(_library);
    }

    void Font::loadFromFile(const std::string &filename) {
        if (FT_New_Face(_library, filename.c_str(), 0, &_face)) {
            throw std::runtime_error("Could not open font");
        }
    }

    void Font::setSize(unsigned int size) {
        _size = size;
        if (_face) {
            FT_Set_Pixel_Sizes(_face, 0, size);
        }
    }

    FT_Face Font::getFace() {
        return _face;
    }


}
