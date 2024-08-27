#include "TDL/Window/AnsiiCode.hpp"


void tdl::AnsiiCode::setRGBFrontGround(Pixel pixel)
{
    _content += "\033[38;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
}

void tdl::AnsiiCode::setRGBBackGround(Pixel pixel)
{
    _content += "\033[48;2;" + std::to_string(GET_R(pixel.color)) + ";" + std::to_string(GET_G(pixel.color)) + ";" + std::to_string(GET_B(pixel.color)) + "m";
}

void tdl::AnsiiCode::clearScreen()
{
    _content += "\033[2J";
}

void tdl::AnsiiCode::moveCursor(Vector2u pos)
{
    _content += "\033[" + std::to_string(pos.y() / 2) + ";" + std::to_string(pos.x()) + "H";
}

void tdl::AnsiiCode::printPixel(const char *shape)
{
    if (shape == nullptr)
        return;
    _content += std::string(reinterpret_cast<const char*>(shape));
}

void tdl::AnsiiCode::alternateScreenBuffer()
{
    _content += "\033[?1049h";
}

void tdl::AnsiiCode::removeMouseCursor()
{
    _content += "\033[?25l";
}

void tdl::AnsiiCode::enableMouseMove()
{
    _content += "\033[?1003h";
    _content += "\033[?1006h";
}

void tdl::AnsiiCode::disableMouseMove()
{
    _content += "\033[?1003l";
    _content += "\033[?1006l";
}

void tdl::AnsiiCode::enableMouseClick()
{
    _content += "\033[?1000h";
}

void tdl::AnsiiCode::disableMouseClick()
{
    _content += "\033[?1000l";
}