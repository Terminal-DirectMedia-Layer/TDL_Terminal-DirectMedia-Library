#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "TDL/Math/Rect.hpp"

TEST_CASE("Rect test", "[Rect]")
{
    SECTION("Rect")
    {
        tdl::RectI rect(0, 0, 10, 10);
        CHECK(rect.x() == 0);
        CHECK(rect.y() == 0);
        CHECK(rect.width() == 10);
        CHECK(rect.height() == 10);
    }

    SECTION("Rect")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(rect);
        CHECK(rect2.x() == 0);
        CHECK(rect2.y() == 0);
        CHECK(rect2.width() == 10);
        CHECK(rect2.height() == 10);
    }

    SECTION("Rect")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(std::move(rect));
        CHECK(rect2.x() == 0);
        CHECK(rect2.y() == 0);
        CHECK(rect2.width() == 10);
        CHECK(rect2.height() == 10);
    }

    SECTION("Rect")
    {
        tdl::RectI rect;
        CHECK(rect.x() == 0);
        CHECK(rect.y() == 0);
        CHECK(rect.width() == 0);
        CHECK(rect.height() == 0);
    }

    SECTION("operator=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2 = rect;
        CHECK(rect2.x() == 0);
        CHECK(rect2.y() == 0);
        CHECK(rect2.width() == 10);
        CHECK(rect2.height() == 10);
    }

    SECTION("operator=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2 = std::move(rect);
        CHECK(rect2.x() == 0);
        CHECK(rect2.y() == 0);
        CHECK(rect2.width() == 10);
        CHECK(rect2.height() == 10);
    }

    SECTION("operator==")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 10);
        CHECK(rect == rect2);
    }

    SECTION("operator!=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 10);
        CHECK_FALSE(rect != rect2);
    }

    SECTION("operator==")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 11);
        CHECK_FALSE(rect == rect2);
    }

    SECTION("operator!=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 11);
        CHECK(rect != rect2);
    }

    SECTION("operator+")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 10);
        tdl::RectI rect3 = rect + rect2;
        CHECK(rect3.x() == 0);
        CHECK(rect3.y() == 0);
        CHECK(rect3.width() == 20);
        CHECK(rect3.height() == 20);
    }

    SECTION("operator-")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 10);
        tdl::RectI rect3 = rect - rect2;
        CHECK(rect3.x() == 0);
        CHECK(rect3.y() == 0);
        CHECK(rect3.width() == 0);
        CHECK(rect3.height() == 0);
    }

    SECTION("operator*")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 10);
        tdl::RectI rect3 = rect * rect2;
        CHECK(rect3.x() == 0);
        CHECK(rect3.y() == 0);
        CHECK(rect3.width() == 100);
        CHECK(rect3.height() == 100);
    }

    SECTION("operator/")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(1, 1, 10, 10);
        tdl::RectI rect3 = rect / rect2;
        CHECK(rect3.x() == 0);
        CHECK(rect3.y() == 0);
        CHECK(rect3.width() == 1);
        CHECK(rect3.height() == 1);
    }

    SECTION("operator+=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 10);
        rect += rect2;
        CHECK(rect.x() == 0);
        CHECK(rect.y() == 0);
        CHECK(rect.width() == 20);
        CHECK(rect.height() == 20);
    }

    SECTION("operator-=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(0, 0, 10, 10);
        rect -= rect2;
        CHECK(rect.x() == 0);
        CHECK(rect.y() == 0);
        CHECK(rect.width() == 0);
        CHECK(rect.height() == 0);
    }

    SECTION("operator*=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        rect *= tdl::RectI(1, 1, 10, 10);
        CHECK(rect.x() == 0);
        CHECK(rect.y() == 0);
        CHECK(rect.width() == 100);
        CHECK(rect.height() == 100);
    }

    SECTION("operator/=")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2(1, 1, 10, 10);
        rect /= rect2;
        CHECK(rect.x() == 0);
        CHECK(rect.y() == 0);
        CHECK(rect.width() == 1);
        CHECK(rect.height() == 1);
    }

    SECTION("Rect &operator=(const Rect &)")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2;
        rect2 = rect;
        CHECK(rect2.x() == 0);
        CHECK(rect2.y() == 0);
        CHECK(rect2.width() == 10);
        CHECK(rect2.height() == 10);
    }

    SECTION("Rect &operator=(Rect &&)")
    {
        tdl::RectI rect(0, 0, 10, 10);
        tdl::RectI rect2;
        rect2 = std::move(rect);
        CHECK(rect2.x() == 0);
        CHECK(rect2.y() == 0);
        CHECK(rect2.width() == 10);
        CHECK(rect2.height() == 10);
    }
}