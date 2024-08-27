// test using catch2

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "TDL/Window/window.hpp"

TEST_CASE("Window test", "[window]")
{
    SECTION("CreateWindow")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        CHECK(window);
        delete window;
    }

    SECTION("clearPixel")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        tdl::Pixel pixel(255, 255, 255, 255);
        window->getMatrix().setPixel(tdl::Vector2u(0, 0), pixel);
        window->clearPixel();
        CHECK(window->getMatrix().getPixel(tdl::Vector2u(0, 0)) == tdl::Pixel(0, 0, 0, 255));
        delete window;
    }

    SECTION("update")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        tdl::Pixel pixel(255, 255, 255, 255);
        window->getMatrix().setPixel(tdl::Vector2u(0, 0), pixel);
        window->draw();
        CHECK(window->getContent().size() == 0);
        window->update();
        CHECK(window->getContent().size() > 0);
        CHECK(window->getContent().find("255;255;255") != std::string::npos);
        delete window;
    }

    SECTION("getContent")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        CHECK(window->getContent().size() == 14);
        delete window;
    }

    SECTION("updateTermSize")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        window->updateTermSize();
        CHECK(window->getMatrix().getSize().x() > 0);
        CHECK(window->getMatrix().getSize().y() > 0);
        delete window;
    }

    SECTION("getHeight")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        CHECK(window->getHeight() > 0);
        delete window;
    }

    SECTION("getWidth")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        CHECK(window->getWidth() > 0);
        delete window;
    }

    SECTION("getFrameRate")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        CHECK(window->getFrameRate() == 60);
        delete window;
    }

    SECTION("setFrameRate")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        window->setFrameRate(30);
        CHECK(window->getFrameRate() == 30);
        delete window;
    }

    SECTION("CreateWindow")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        write(window->getFd(), "A", 1);
        for(tdl::Event event; window->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KeyPressed) {
                CHECK(event.key.code == tdl::KeyCodes::KEY_A);
            }
        }
    }

    SECTION("poll the release event")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        write(window->getFd(), "A", 1);
        for(tdl::Event event; window->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KeyPressed) {
                CHECK(event.key.code == tdl::KeyCodes::KEY_A);
            }
        }
        for(tdl::Event event; window->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KeyReleased) {
                CHECK(event.key.code == tdl::KeyCodes::KEY_A);
            }
        }
    }

    SECTION("poll the event with arrow event")
    {
        tdl::Window *window = tdl::Window::CreateWindow("test");
        write(window->getFd(), "^[[A", 4);
        for(tdl::Event event; window->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KeyPressed) {
                CHECK(event.key.code == tdl::KeyCodes::KEY_UP);
            }
        }
    }

}