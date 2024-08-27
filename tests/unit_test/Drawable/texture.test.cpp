#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "TDL/Drawable/Texture/Texture.hpp"

TEST_CASE("Texture test", "[Texture]")
{
    SECTION("CreateTexture")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        CHECK(texture != nullptr);
        //delete texture;
    }

    SECTION("CreateTexture")
    {
        std::string path = "../example/assets/bird.png";
        tdl::Texture *texture = tdl::Texture::CreateTexture(path, true);
        CHECK(texture != nullptr);
        //delete texture;
    }

    SECTION("throw CreateTexture")
    {
        std::string path = "wrong_path";
        tdl::Texture *texture = tdl::Texture::CreateTexture(path, true);
        CHECK(texture == nullptr);
        
        //delete texture;
    }

    SECTION("CreateTextureFromVector")
    {
        tdl::Pixel pixelData[100];
        tdl::Vector2u size(10, 10);
        tdl::Texture *texture = tdl::Texture::CreateTextureFromVector(pixelData, size);
        CHECK(texture != nullptr);
        //delete texture;
    }

    SECTION("CreateTextureFromVector")
    {
        tdl::Pixel pixelData[100];
        tdl::Vector2u size(10, 10);
        tdl::Vector2f scale(1.0, 1.0);
        tdl::Texture *texture = tdl::Texture::CreateTextureFromVector(pixelData, size, scale);
        CHECK(texture != nullptr);
        //delete texture;
    }

    SECTION("CreateTextureFromVector")
    {
        tdl::Pixel pixelData[100];
        tdl::Vector2u size(10, 10);
        tdl::Vector2f scale(1.0, 1.0);
        tdl::Texture *texture = tdl::Texture::CreateTextureFromVector(pixelData, size, true);
        CHECK(texture != nullptr);
        //delete texture;
    }

    SECTION("CreateTextureFromVector")
    {
        tdl::Pixel pixelData[100];
        tdl::Vector2u size(10, 10);
        tdl::Vector2f scale(1.0, 1.0);
        tdl::Texture *texture = tdl::Texture::CreateTextureFromVector(pixelData, size, scale, true);
        CHECK(texture != nullptr);
        //delete texture;
    }

    SECTION("getOriginalPixel")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        tdl::Vector2u size(0, 0);
        tdl::Pixel pixel = tdl::Pixel(0, 0, 0, 0);
        pixel = texture->getOriginalPixel(size);
        CHECK(pixel != tdl::Pixel(0, 0, 0, 0));
        //delete texture;
    }

    SECTION("getOriginalImageData")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        tdl::PixelMatrix pixel = texture->getOriginalImageData();
        CHECK(pixel.getSize() != tdl::Vector2u(0, 0));
        //delete texture;
    }

    SECTION("setRect")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        tdl::RectU rect(0, 0, 10, 10);
        texture->setRect(rect);
        CHECK(texture->getRect().value() == rect);
        //delete texture;
    }

    SECTION("setRect")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        tdl::RectU rect(0, 0, 10, 10);
        texture->setRect(0, 0, 10, 10);
        CHECK(texture->getRect().value() == rect);
        //delete texture;
    }

    SECTION("getRect")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        tdl::RectU rect(0, 0, 10, 10);
        texture->setRect(rect);
        CHECK(texture->getRect().value() == rect);
        //delete texture;
    }

    SECTION("getSize")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        tdl::Vector2u size = texture->getSize();
        CHECK(size != tdl::Vector2u(0, 0));
        //delete texture;
    }

    SECTION("resetRect")
    {
        tdl::Texture *texture = tdl::Texture::CreateTexture("../example/assets/bird.png");
        tdl::RectU rect(0, 0, 10, 10);
        texture->setRect(rect);
        CHECK(texture->getRect().has_value() == true);
        texture->resetRect();
        CHECK(texture->getRect().has_value() == false);
        //delete texture;
    }
}

