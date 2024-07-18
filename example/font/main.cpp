

#include <tdl/Window/window.hpp>
#include "tdl/Text/Font/Font.hpp"
#include "tdl/Text/Text.hpp"
#include "tdl/Matrix/Transformation.hpp"
#include "tdl/sound/Sound.hpp"



int main()
{
    tdl::Window *win = tdl::Window::CreateWindow("font");
    tdl::Font font;
    font.loadFromFile("../example/assets/font.ttf");
    std::string s;
    tdl::Text text(font, s);
    float x = 0;
    text.setRotation(x);
    text.setScale(1, 1);
    font.setSize(24);
    text.setColor(tdl::Pixel(255, 0, 255, 255));
    while (true)
    {
        win->clearPixel();
        text.setText(s);
        //text.setRotation(x);
        text.draw(win);
        text.setPosition(100, 45);
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KeyPressed) {
                if (event.key.code == tdl::KeyCodes::KEY_ESC) {
                    delete win;
                    return 0;
                }
                if (event.key.code == tdl::KeyCodes::KEY_TAB && !s.empty())
                    s.pop_back();
                else
                    s.push_back(event.key.code);
            }
        }
        x += 10;
        win->printFrameRate();
    }

}