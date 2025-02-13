
#include <string>
#include <vector>
#include <iostream>

#include <png.h>

#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Graphics/Drawable/Sprite/Texture/Loader/TextureLoader.hpp"

tdl::TextureLoader::TextureLoader(std::string path) : _path(path), _info_ptr(nullptr),
                                                      _row_pointers(nullptr), _png_ptr(nullptr),
                                                      _size(0, 0), _channels(0), _bit_depth(0)
{
    if(path != "")
        loadTexture();
}

tdl::TextureLoader::~TextureLoader()
{
    png_destroy_read_struct(&_png_ptr, &_info_ptr, nullptr);
}

bool tdl::TextureLoader::isPng(std::string path)
{
    FILE* file = fopen(path.c_str(), "rb");
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    png_byte header[8];
    fread(header, 1, 8, file);
    fclose(file);

    return !png_sig_cmp(header, 0, 8);
}

void tdl::TextureLoader::loadTexture(std::string path)
{
    _path = path;
    FILE *fp = fopen(_path.c_str(), "rb");
    if (!fp)
        throw std::runtime_error("Failed to open file");
    if (!isPng(path))
        throw std::runtime_error("File is not a png");
    _png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    _info_ptr = png_create_info_struct(_png_ptr);
    png_init_io(_png_ptr, fp);
    png_read_png(_png_ptr, _info_ptr, PNG_TRANSFORM_GRAY_TO_RGB, NULL);
    _size = Vector2u(png_get_image_width(_png_ptr, _info_ptr), png_get_image_height(_png_ptr, _info_ptr));
    _channels = png_get_channels(_png_ptr, _info_ptr);
    _bit_depth = png_get_bit_depth(_png_ptr, _info_ptr);
    _row_pointers = png_get_rows(_png_ptr, _info_ptr);
    fclose(fp);
}

void tdl::TextureLoader::loadTexture()
{
    loadTexture(_path);
}