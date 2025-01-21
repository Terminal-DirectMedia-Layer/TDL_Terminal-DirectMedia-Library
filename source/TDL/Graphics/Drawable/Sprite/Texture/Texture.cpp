
#include <iostream>
#include <utility>

#include "TDL/Graphics/Drawable/Sprite/Texture/Texture.hpp"
#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Math/Rect.hpp"
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Utils/Memory/Arena.hpp"

namespace tdl {

    Texture::Texture(std::string path) : TextureLoader(path)
    {
        loadPixels();
    }

    Texture::Texture(Pixel *pixelData, Vector2u &size) : TextureLoader("")
    {
        _originalImageData = Matrix<Pixel>(size);
        for (u_int32_t y = 0; y < size.y(); y++) {
            for (u_int32_t x = 0; x < size.x(); x++) {
                _originalImageData.setElement(Vector2u(x, y), pixelData[y * size.x() + x]);
            }
        }
        _size = size;
    }

    Texture::~Texture() = default;
    
    void Texture::loadPixels()
    {
        Pixel color;
        png_byte *ptr;
        if (!_originalImageData.empty())
            _originalImageData.clear();
        _originalImageData.resize(_size);
        for (u_int32_t y = 0; y < _size.y(); y++) {
            std::vector<Pixel> row;
            for (u_int32_t x = 0; x < _size.x(); x++) {
                ptr = &(_row_pointers[y][x * _channels]);
                if (_channels == 3 || _channels == 4) {
                    color = Pixel(ptr[0], ptr[1], ptr[2], _channels == 4 ? ptr[3] : 255);
                } else if (_channels == 2) {
                    color = Pixel(ptr[0], ptr[0], ptr[0], ptr[1]);
                } else if (_channels == 1) {
                    color = Pixel(ptr[0], ptr[0], ptr[0], 255);
                }
                _originalImageData.setElement(Vector2u(x, y), color);
            }
        }
    }

    Pixel Texture::getOriginalPixel(Vector2i &pos)
    {
        return _originalImageData.getElement(pos);
    }
}