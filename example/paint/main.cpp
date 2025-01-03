

#include <TDL/Graphics/Display/Display.hpp>
#include <TDL/Graphics/Window/Window.hpp>
#include <TDL/Graphics/Drawable/Sprite/Sprite.hpp>


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

void mousecallback(tdl::Event &event) {
    if (event.type == TDL_MOUSEMOVED) {
        tdl::Display::getInstance()._cursor.setPosition(tdl::Vector2u(event.mouseMove.x , event.mouseMove.y));
    }
}

int main()
{
    const int targetFPS = 120;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);

    tdl::Window win("test", tdl::Vector2u(1000,300), tdl::Vector2u(200,200), tdl::Pixel(255,0,0,255));
    tdl::Window win2("test2", tdl::Vector2u(1200,700), tdl::Vector2u(350,300));
    tdl::Window win3("test3", tdl::Vector2u(1000,700), tdl::Vector2u(550,400), tdl::Pixel(0,255,0,255));
    tdl::Texture tex("../example/assets/bird.png");
    tdl::RectU rect(0,0,32,32);
    tdl::Sprite sprite(&tex, tdl::Vector2u(10,10));
    tdl::Texture spiner("../example/assets/Spinner.png");
    tdl::Sprite sprite2(&spiner, tdl::Vector2u(0,0),rect);
    win.addDrawable(&sprite);
    win2.addDrawable(&sprite2);
    sprite2.setScale(2.0,2.0);
    //tdl::Display::getInstance().addWindow(&win);
    //tdl::Display::getInstance().addWindow(&win2);
    //tdl::Display::getInstance().addWindow(&win3);
    win.onEvent = eventWin1;
    tdl::Display::getInstance(tdl::DisplayType::TTY).onEvent = eventDisplay;

    tdl::Display::getInstance()._interruptManager.start();
    tdl::Display::getInstance()._interruptManager.addInterrupt(mousecallback,TDL_MOUSEMOVED, 1);
    tdl::Display::getInstance()._interruptManager.addInterrupt(eventDisplay,TDL_KEYPRESSED, 2);

    int i = 0;

    while (tdl::Display::getInstance().isOpen()) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        tdl::Display::getInstance().clear();

        if (rect.x() >= 32) {
            rect.x() = 0;
            if (rect.y() >= 96) {
                rect.y() = 0;
            } else {
                rect.y() += 32;
            }
        } else {
            rect.x() += 32;
        }
        //sprite2.setRext(rect);
        //tdl::Display::getInstance().pollEvent();
        tdl::Display::getInstance().draw();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = frameEnd - frameStart;
        if (elapsed < frameDuration) {
            std::this_thread::sleep_for(frameDuration - elapsed);
        }

        tdl::Display::getInstance().fps.frameRendered();

        //i++;
    }
    return 0;
}
