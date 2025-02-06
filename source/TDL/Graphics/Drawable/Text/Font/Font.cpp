
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <iostream>
#include "TDL/Graphics/Drawable/Text/Font/Font.hpp"

namespace tdl {
    Font::Font() : _size(12), _face(nullptr) {
        if (FT_Init_FreeType(&_library)) {
            std::cerr << "Error: Could not init freetype library" << std::endl;
        }
    }

    Font::~Font() {
        if (_face) {
            FT_Done_Face(*_face);
        }
        FT_Done_FreeType(_library);
    }

    void Font::loadFromFile(const std::string &filename) {
        FT_Face face;
        if (FT_New_Face(_library, filename.c_str(), 0, &face)) {
            throw std::runtime_error("Could not open font");
        }
        _face = std::shared_ptr<FT_Face>(new FT_Face(face), [](FT_Face* f) { FT_Done_Face(*f); delete f; });
    }

    void Font::setSize(unsigned int size) {
        _size = size;
        if (FT_Set_Pixel_Sizes(*_face, size, size)) {
            throw std::runtime_error("Could not set font size");
        }
    }

    FT_Face Font::getFace() {
        return *_face;
    }
}