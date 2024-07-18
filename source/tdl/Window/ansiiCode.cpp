#include "tdl/Window/ansiiCode.hpp"


void tdl::ansiiCode::setRGBFrontGround(Pixel pixel)
{
    _content += "\033[38;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
}

void tdl::ansiiCode::setRGBBackGround(Pixel pixel)
{
    _content += "\033[48;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
}

void tdl::ansiiCode::clearScreen()
{
    _content += "\033[2J";
}

void tdl::ansiiCode::moveCursor(Vector2u pos)
{
    _content += "\033[" + std::to_string(pos.y() / 2) + ";" + std::to_string(pos.x()) + "H";
}

void tdl::ansiiCode::printPixel(const char *shape)
{
    if (shape == nullptr)
        return;
    _content += std::string(reinterpret_cast<const char*>(shape));
}

void tdl::ansiiCode::alternateScreenBuffer()
{
    _content += "\033[?1049h";
}

void tdl::ansiiCode::removeMouseCursor()
{
    _content += "\033[?25l";
}

void tdl::ansiiCode::enableMouseMove()
{
    _content += "\033[?1003h";
    _content += "\033[?1006h";
}

void tdl::ansiiCode::disableMouseMove()
{
    _content += "\033[?1003l";
    _content += "\033[?1006l";
}

void tdl::ansiiCode::enableMouseClick()
{
    _content += "\033[?1000h";
}

void tdl::ansiiCode::disableMouseClick()
{
    _content += "\033[?1000l";
}