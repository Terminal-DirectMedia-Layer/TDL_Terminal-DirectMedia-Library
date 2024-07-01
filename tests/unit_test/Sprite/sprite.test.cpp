#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "tdl/Sprite/Sprite.hpp"
#include "tdl/Window/window.hpp"

TEST_CASE("Sprite test", "[Sprite]")
{
    SECTION("createSprite")
    {
        tdl::Texture *texture = tdl::Texture::createTexture("../example/assets/bird.png");
        tdl::Sprite *sprite = tdl::Sprite::createSprite(texture, tdl::Vector2u(0, 0));
        CHECK(sprite != nullptr);
        //delete sprite;
    }

    SECTION("createSprite")
    {
        tdl::Texture *texture = tdl::Texture::createTexture("../example/assets/bird.png");
        tdl::RectU rect(0, 0, 10, 10);
        tdl::Vector2u pos(0, 0);
        tdl::Sprite *sprite = tdl::Sprite::createSprite(texture, pos, rect);
        CHECK(sprite != nullptr);
        //delete sprite;
    }

    SECTION("createSprite")
    {
        tdl::Texture *texture = tdl::Texture::createTexture("../example/assets/bird.png");
        tdl::Sprite *sprite = tdl::Sprite::createSprite(texture, tdl::Vector2u(0, 0));
        tdl::Sprite *sprite2 = tdl::Sprite::createSprite(texture, tdl::Vector2u(0, 0));
        CHECK(sprite != sprite2);
        //delete sprite;
        //delete sprite2;
    }

    SECTION("setTexture")
    {
        tdl::Texture *texture = tdl::Texture::createTexture("../example/assets/bird.png");
        tdl::Sprite *sprite = tdl::Sprite::createSprite(texture, tdl::Vector2u(0, 0));
        tdl::Texture *texture2 = tdl::Texture::createTexture("../example/assets/bird.png");
        sprite->setTexture(texture2);
        CHECK(sprite->getTexture() == texture2);
        //delete sprite;
    }

    SECTION("setTint")
    {
        tdl::Texture *texture = tdl::Texture::createTexture("../example/assets/bird.png");
        tdl::Sprite *sprite = tdl::Sprite::createSprite(texture, tdl::Vector2u(0, 0));
        sprite->setTint(tdl::Pixel(255, 255, 255, 255));
        CHECK(sprite->getTint() == tdl::Pixel(255, 255, 255, 255));
        //delete sprite;
    }

    SECTION("draw")
    {
        tdl::Texture *texture = tdl::Texture::createTexture("../example/assets/bird.png");
        tdl::Sprite *sprite = tdl::Sprite::createSprite(texture, tdl::Vector2u(0, 0));
        tdl::Window *window = tdl::Window::CreateWindow("Test");
        CHECK(window);
        CHECK(sprite);
        CHECK(texture);
        tdl::Vector2u pos(0, 0);
        sprite->draw(window);
        CHECK(window->getMatrix().getPixel(pos) != tdl::Pixel(0, 0, 0, 0));
        //delete sprite;
    }
}