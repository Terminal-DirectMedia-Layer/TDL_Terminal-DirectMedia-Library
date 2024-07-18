
#include <iostream>
#include <utility>
#include "tdl/Drawable/Texture/Texture.hpp"
#include "tdl/Vector.hpp"
#include "tdl/Rect.hpp"
#include "tdl/Pixel/Pixel.hpp"
#include "tdl/Drawable/Drawable.hpp"

namespace tdl {

    Texture *Texture::createTexture(std::string path)
    {
        return createTexture(path, false);
    }

    Texture *Texture::createTexture(std::string &path, bool repeat)
    {
        try {
            return new Texture(path, repeat);
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        return nullptr;
    }

    Texture::Texture(std::string &path, bool repeat) : TextureLoader(path), Transformable()
    {
        loadPixels();
    }

    Texture *Texture::createTextureFromVector(Pixel *pixelData, Vector2u &size)
    {
        Vector2f scale = Vector2f(1.0, 1.0);
        return createTextureFromVector(pixelData, size, scale, false);
    }

    Texture *Texture::createTextureFromVector(Pixel *pixelData, Vector2u &size, Vector2f &scale)
    {
        return createTextureFromVector(pixelData, size, scale, false);
    }

    Texture *Texture::createTextureFromVector(Pixel *pixelData, Vector2u &size, bool repeat)
    {
        Vector2f scale = Vector2f(1.0, 1.0);
        return createTextureFromVector(pixelData, size, scale, repeat);
    }

    Texture *Texture::createTextureFromVector(Pixel *pixelData, Vector2u &size, Vector2f &scale, bool repeat)
    {
        return new Texture(pixelData, size, scale, repeat);
    }

    Texture::Texture(Pixel *pixelData, Vector2u &size, tdl::Vector2f &scale, bool repeat) : TextureLoader("")
    {
        _originalImageData = PixelMatrix(size);
        for (u_int32_t y = 0; y < size.y(); y++) {
            for (u_int32_t x = 0; x < size.x(); x++) {
                _originalImageData.setPixel(Vector2u(x, y), pixelData[y * size.x() + x]);
            }
        }
        _size = size;
    }

    Texture::~Texture() = default;
    
    void Texture::loadPixels()
    {
        Pixel color;
        png_byte *ptr;
        if (!_originalImageData.getPixelsTab().empty())
            _originalImageData.clear();
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
                row.push_back(color);
            }
            _originalImageData.append(row);
        }
    }

    Pixel Texture::getOriginalPixel(Vector2u &pos)
    {
        return _originalImageData.getPixel(pos);
    }
}