

#include <TDL/Window/window.hpp>
#include <TDL/Sprite/Sprite.hpp>

#include "TDL/Matrix/Transformation.hpp"
#include <regex>



int main()
{
    tdl::Window *win = tdl::Window::CreateWindow("font");
    bool isClicked = false;
    float x = 0;
    std::vector<tdl::Pixel> p;
    p.resize(1000000, tdl::Pixel(0, 0, 0, 255));
    tdl::Vector2u pos = tdl::Vector2u(1000, 1000);
    tdl::Texture *tex = tdl::Texture::CreateTextureFromVector(p.data(), pos);
    tdl::Sprite *sprite = tdl::Sprite::createSprite(tex, tdl::Vector2u(0, 0));
    while (true)
    {
        win->clearPixel();
        sprite->draw(win);
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::MouseButtonPressed) {
                isClicked = true;
            }
            if (event.type == tdl::Event::EventType::MouseButtonReleased) {
                isClicked = false;
            }
            if (event.type == tdl::Event::EventType::MouseMoved) {
                if (isClicked) {
                    tdl::Vector2u pos = tdl::Vector2u(event.mouseMove.x, event.mouseMove.y);
                    tdl::Pixel p = tdl::Pixel(255, 255, 255, 255);
                    tex->getOriginalImageData().setPixel(pos, p);
                    sprite->setTexture(tex);
                }
            }
        }
        x += 10;
        win->printFrameRate();
    }
}