
#include <TDL/Window/TtyDisplay.hpp>
#include "TDL/Text/Font/Font.hpp"
#include "TDL/Text/Text.hpp"
#include "TDL/Matrix/Transformation.hpp"
#include "TDL/Sound/Sound.hpp"

int main()
{
    tdl::TtyDisplay *win = tdl::TtyDisplay::createTtyDisplay("font");
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
        text.draw(win);
        text.setPosition(100, 45);
        win->update();
        win->draw();

        for(tdl::Event event; win->pollEvent(event);) {
            if (event.type == TDL_KEYPRESSED) {
                if (event.key == TDL_KEY_ESC) {
                    delete win;
                    return 0;
                }
                if (event.key == TDL_KEY_DELETE && !s.empty())
                    s.pop_back();
                else
                    s.push_back(event.key.code[0]);
            }
        }
        win->printFrameRate();
    }

}