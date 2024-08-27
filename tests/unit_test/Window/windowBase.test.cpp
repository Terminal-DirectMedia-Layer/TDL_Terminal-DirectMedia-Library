#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "TDL/Window/windowBase.hpp"
#include "TDL/Event/Event.hpp"

TEST_CASE("windowBase test", "[windowBase]")
{
    SECTION("WindowBase")
    {
        tdl::WindowBase *windowBase = new tdl::WindowBase("test");
        CHECK(windowBase);
        delete windowBase;
    }

    SECTION("pollEvent")
    {
        tdl::WindowBase *windowBase = new tdl::WindowBase("test");
        tdl::Event event;
        CHECK(windowBase->pollEvent(event) == false);
        delete windowBase;
    }

    SECTION("pushEvent")
    {
        tdl::WindowBase *windowBase = new tdl::WindowBase("test");
        tdl::Event event;
        event.type = tdl::Event::EventType::KeyPressed;
        event.key.code = tdl::KeyCodes::KEY_A;
        windowBase->pushEvent(event);
        CHECK(windowBase->pollEvent(event) == true);
        CHECK(event.type == tdl::Event::EventType::KeyPressed);
        CHECK(event.key.code == tdl::KeyCodes::KEY_A);
        delete windowBase;
    }

    SECTION("getFd")
    {
        tdl::WindowBase *windowBase = new tdl::WindowBase("test");
        CHECK(windowBase->getFd() == 0);
        delete windowBase;
    }
}