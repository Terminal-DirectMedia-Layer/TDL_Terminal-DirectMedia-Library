
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"

namespace tdl {

    bool Pixel::operator==(const Pixel &other) const {
        return color == other.color;
    }

    bool Pixel::operator!=(const Pixel &other) const {
        return color != other.color;
    }

    Pixel Pixel::operator+(const Pixel &other) const {
        if (GET_A(other.color) == 0)
            return *this;
        float alpha = GET_A(color) / 255.0;
        uint8_t r = GET_R(color) * (1 - alpha) + GET_R(other.color) * alpha;
        uint8_t g = GET_G(color) * (1 - alpha) + GET_G(other.color) * alpha;
        uint8_t b = GET_B(color) * (1 - alpha) + GET_B(other.color) * alpha;
        uint8_t a = std::clamp(static_cast<uint8_t>(GET_A(color) + GET_A(other.color)), std::uint8_t(0), std::uint8_t(255));
        return Pixel(r, g, b, a);
    }

    Pixel Pixel::operator-(const Pixel &other) const {
        if (GET_A(other.color) == 0)
            return *this;
        float alpha = GET_A(color) / 255.0;
        uint8_t r = GET_R(color) * (1 - alpha) - GET_R(other.color) * alpha;
        uint8_t g = GET_G(color) * (1 - alpha) - GET_G(other.color) * alpha;
        uint8_t b = GET_B(color) * (1 - alpha) - GET_B(other.color) * alpha;
        uint8_t a = std::clamp(static_cast<uint8_t>(GET_A(color) - GET_A(other.color)), std::uint8_t(0), std::uint8_t(255));
        return Pixel(r, g, b, a);
    }

    bool Pixel::operator<(const Pixel& other) const {
        return color < other.color;
    }
}