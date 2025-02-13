#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_tostring.hpp>
#include <ostream>
#include <sstream>

#include "TDL/Utils/Math/Vector.hpp"

TEST_CASE("Vector test", "[Vector]")
{
    SECTION("Vector")
    {
        tdl::Vector2i vec(0, 0);
        CHECK(vec.x() == 0);
        CHECK(vec.y() == 0);
    }

    SECTION("Vector")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(vec);
        CHECK(vec2.x() == 0);
        CHECK(vec2.y() == 0);
    }

    SECTION("Vector")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(std::move(vec));
        CHECK(vec2.x() == 0);
        CHECK(vec2.y() == 0);
    }

    SECTION("Vector")
    {
        tdl::Vector2i vec;
        CHECK(vec.x() == 0);
        CHECK(vec.y() == 0);
    }

    SECTION("operator=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2 = vec;
        CHECK(vec2.x() == 0);
        CHECK(vec2.y() == 0);
    }

    SECTION("operator=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2;
        vec2 = vec;
        CHECK(vec2.x() == 0);
        CHECK(vec2.y() == 0);
    }

    SECTION("operator=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2;
        vec2 = std::move(vec);
        CHECK(vec2.x() == 0);
        CHECK(vec2.y() == 0);
    }

    SECTION("operator=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2;
        vec2 = std::move(vec);
        CHECK(vec2.x() == 0);
        CHECK(vec2.y() == 0);
    }

    SECTION("operator==")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(0, 0);
        CHECK(vec == vec2);
    }

    SECTION("operator==")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(1, 1);
        CHECK_FALSE(vec == vec2);
    }

    SECTION("operator!=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(0, 0);
        CHECK_FALSE(vec != vec2);
    }

    SECTION("operator!=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(1, 1);
        CHECK(vec != vec2);
    }

    SECTION("operator+")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(1, 1);
        tdl::Vector2i vec3 = vec + vec2;
        CHECK(vec3.x() == 1);
        CHECK(vec3.y() == 1);
    }

    SECTION("operator-")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(1, 1);
        tdl::Vector2i vec3 = vec - vec2;
        CHECK(vec3.x() == -1);
        CHECK(vec3.y() == -1);
    }

    SECTION("operator*")
    {
        tdl::Vector2i vec(1, 1);
        tdl::Vector2i vec2(2, 2);
        tdl::Vector2i vec3 = vec * vec2;
        CHECK(vec3.x() == 2);
        CHECK(vec3.y() == 2);
    }

    SECTION("operator/")
    {
        tdl::Vector2i vec(2, 2);
        tdl::Vector2i vec2(1, 1);
        tdl::Vector2i vec3 = vec / vec2;
        CHECK(vec3.x() == 2);
        CHECK(vec3.y() == 2);
    }

    SECTION("operator+=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(1, 1);
        vec += vec2;
        CHECK(vec.x() == 1);
        CHECK(vec.y() == 1);
    }

    SECTION("operator-=")
    {
        tdl::Vector2i vec(0, 0);
        tdl::Vector2i vec2(1, 1);
        vec -= vec2;
        CHECK(vec.x() == -1);
        CHECK(vec.y() == -1);
    }

    SECTION("operator*=")
    {
        tdl::Vector2i vec(1, 1);
        tdl::Vector2i vec2(2, 2);
        vec *= vec2;
        CHECK(vec.x() == 2);
        CHECK(vec.y() == 2);
    }

    SECTION("operator/=")
    {
        tdl::Vector2i vec(2, 2);
        tdl::Vector2i vec2(1, 1);
        vec /= vec2;
        CHECK(vec.x() == 2);
        CHECK(vec.y() == 2);
    }

    SECTION("dot")
    {
        tdl::Vector2i vec(1, 1);
        tdl::Vector2i vec2(2, 2);
        CHECK(vec.dot(vec2) == 4);
    }

    SECTION("cross")
    {
        tdl::Vector2i vec(1, 1);
        tdl::Vector2i vec2(2, 2);
        CHECK(vec.cross(vec2) == 0);
    }

    SECTION("operator<=")
    {
        tdl::Vector2i vec(1, 1);
        tdl::Vector2i vec2(2, 2);
        CHECK(vec <= vec2);
    }

    SECTION("operator<=")
    {
        tdl::Vector2i vec(2, 2);
        tdl::Vector2i vec2(1, 1);
        CHECK_FALSE(vec <= vec2);
    }

    SECTION("operator>=")
    {
        tdl::Vector2i vec(2, 2);
        tdl::Vector2i vec2(1, 1);
        CHECK(vec >= vec2);
    }

    SECTION("operator>=")
    {
        tdl::Vector2i vec(1, 1);
        tdl::Vector2i vec2(2, 2);
        CHECK_FALSE(vec >= vec2);
    }

    
}
