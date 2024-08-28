    #include <iostream>
#include <unistd.h>
#include <TDL/Window/TtyDisplay.hpp>
#include <TDL/Drawable/Texture/Texture.hpp>
#include <TDL/Sprite/Sprite.hpp>
#include "TDL/Input/InputKeyboard.hpp"

int main()
{
    tdl::TtyDisplay *win = tdl::TtyDisplay::createTtyDisplay("WindowName");
    tdl::Texture *tex = tdl::Texture::createTexture("../example/assets/tdl_png_logo.png");
    tdl::Texture *tex2 = tdl::Texture::createTexture("../example/assets/circle.png");
    tdl::Sprite *sprite = tdl::Sprite::createSprite(tex, tdl::Vector2u(0, 0));
    tdl::Sprite *sprite2 = tdl::Sprite::createSprite(tex2, tdl::Vector2u(0, 0));
    int i = 0;
	sprite->setPosition(500, 500);
	while (true)
    {
        win->clearPixel();
        sprite->draw(win);
        sprite2->draw(win);
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KEYPRESSED) {
                if (event.key.code == tdl::KeyCodes::KEY_ESC) {
                    delete win;
                    delete sprite;
                    return 0;
                }
            }
            if (event.type == tdl::Event::EventType::MOUSEMOVED) {
                sprite2->setPosition(event.mouseMove.x, event.mouseMove.y);
            }
            if (event.type == tdl::Event::EventType::MOUSEBUTTONPRESSED) {
                if (event.mouseButton.button == tdl::MouseButton::LEFT) {
                    sprite2->setScale(sprite2->getScale() + tdl::Vector2f(0.1, 0.1));
                }
                if (event.mouseButton.button == tdl::MouseButton::RIGHT) {
                    sprite2->setScale(sprite2->getScale() - tdl::Vector2f(0.1, 0.1));
                }
            }
        }
        win->printFrameRate();
    }
    return 0;
}