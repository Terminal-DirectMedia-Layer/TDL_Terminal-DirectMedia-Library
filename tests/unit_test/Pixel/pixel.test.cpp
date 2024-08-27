#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "TDL/Pixel/Pixel.hpp"

TEST_CASE("Pixel test", "[Pixel]")
{
    SECTION("Pixel")
    {
        tdl::Pixel pixel(255, 255, 255, 255);
        CHECK(GET_R(pixel.color) == 255);
        CHECK(GET_G(pixel.color) == 255);
        CHECK(GET_B(pixel.color) == 255);
        CHECK(GET_A(pixel.color) == 255);
    }

    SECTION("Pixel")
    {
        tdl::Pixel pixel(255, 255, 255, 100);
        CHECK(GET_R(pixel.color) == 255);
        CHECK(GET_G(pixel.color) == 255);
        CHECK(GET_B(pixel.color) == 255);
        CHECK(GET_A(pixel.color) == 100);
    }

    SECTION("operator +")
    {
        tdl::Pixel pixel(100, 100, 100, 255);
        tdl::Pixel pixel2(100, 100, 100, 255);
        tdl::Pixel pixel3 = pixel + pixel2;
        CHECK(GET_R(pixel3.color) == 100);
        CHECK(GET_G(pixel3.color) == 100);
        CHECK(GET_B(pixel3.color) == 100);
        CHECK(GET_A(pixel3.color) == 254);
    }

        SECTION("operator +")
    {
        tdl::Pixel pixel(100, 100, 100, 255);
        tdl::Pixel pixel2(100, 100, 100, 0);
        tdl::Pixel pixel3 = pixel + pixel2;
        CHECK(GET_R(pixel3.color) == 100);
        CHECK(GET_G(pixel3.color) == 100);
        CHECK(GET_B(pixel3.color) == 100);
        CHECK(GET_A(pixel3.color) == 255);
    }

    SECTION("operator -")
    {
        tdl::Pixel pixel(100, 100, 100, 0);
        tdl::Pixel pixel2(100, 100, 100, 0);
        tdl::Pixel pixel3 = pixel - pixel2;
        CHECK(GET_R(pixel3.color) == 100);
        CHECK(GET_G(pixel3.color) == 100);
        CHECK(GET_B(pixel3.color) == 100);
        CHECK(GET_A(pixel3.color) == 0);
    }
}
