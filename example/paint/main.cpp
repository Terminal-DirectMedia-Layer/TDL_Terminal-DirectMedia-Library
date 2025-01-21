

#include <TDL/Graphics/Display/Display.hpp>
#include <TDL/Graphics/Window/Window.hpp>
#include <TDL/Graphics/Drawable/Sprite/Sprite.hpp>
#include <TDL/Graphics/Drawable/Text/Text.hpp>

#define TRACY_ENABLE 1

void eventWin1(tdl::Event &event, tdl::Window *win) {
    if (event.type == TDL_MOUSEPRESSED && win->isClickIn(tdl::Vector2u(event.mouseButton.x, event.mouseButton.y)) == tdl::INSIDE) {
        for (u_int32_t y = 0; y < win->getSize().y(); y++) {
            for (u_int32_t x = 0; x < win->getSize().x(); x++) {
                tdl::Pixel p = win->getPixel(x, y) + tdl::Pixel(0, 255, 10, 255);
                win->setPixel(x, y, p);
            }
        }
    }
}

void eventDisplay(tdl::Event &event) {
    if (event.type == TDL_KEYPRESSED) {
        if (event.key == TDL_KEY_A) {
            tdl::Display::getInstance().close();
        }
    }
}

int main()
{
    const int targetFPS = 120;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    double rot = 0.0f;
    tdl::Window win("test", tdl::Vector2u(1000,300), tdl::Vector2u(10,100), tdl::Pixel(255,0,0,255));
    tdl::Window win2("test2", tdl::Vector2u(1200,700), tdl::Vector2u(350,300));
    tdl::Window win3("test3", tdl::Vector2u(1000,700), tdl::Vector2u(550,400), tdl::Pixel(0,255,0,255));
    tdl::Window win4("test4", tdl::Vector2u(1000,700), tdl::Vector2u(600,400));

    tdl::Texture tex("../example/assets/bird.png");
    tdl::RectU rect(0,0,93,107);
    tdl::Sprite sprite(&tex, tdl::Vector2u(40,40));
    tdl::Texture spiner("../example/assets/nombre.png");
    tdl::Sprite sprite2(&spiner, tdl::Vector2u(50,50),rect);

    tdl::Font f;
    f.loadFromFile("../example/assets/font.ttf");
    //tdl::Text text(f, "Hello World");

    win.addDrawable(&sprite);
    win2.addDrawable(&sprite2);
    sprite2.setPosition(70,70);

    tdl::Display::getInstance().addWindow(&win);
    tdl::Display::getInstance().addWindow(&win2);
    tdl::Display::getInstance().addWindow(&win3);
    tdl::Display::getInstance().addWindow(&win4);

    win.onEvent = eventWin1;

    tdl::Display::getInstance()._interruptManager.start();
    tdl::Display::getInstance()._interruptManager.addInterrupt(eventDisplay,TDL_KEYPRESSED, 2);

    int i = 0;
    while (tdl::Display::getInstance().isOpen()) {

        tdl::Display::getInstance().clear();
        if (rect.x() > 903) {
            rect.x() = 0;
        } else {
            rect.x() += 93;
        }
        sprite2.setRext(rect);
        // //sprite2.setRotation(rot);
        // rot += 1.0f;
        tdl::Display::getInstance().pollEvent();
        tdl::Display::getInstance().draw();
        tdl::Display::getInstance().fps.frameRendered();
        i++;
    }
    return 0;
}
