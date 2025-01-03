#include "TDL/Graphics/Display/Display.hpp"
#include "TDL/Graphics/Widget/Widget.hpp"
#include "TDL/Utils/Parser/Json.hpp"


namespace tdl {

    Widget::Widget(std::string name, std::string file, Vector2u pos) : Transformable()
    {
        _name = name;
        setPosition(pos);
        JsonParser parser;
        parser.parseFile(file);
         auto bufsize = parser.key_value("size");
         if (!bufsize.is_array() && bufsize.size() == 2) {
             throw std::runtime_error("Size is not an 2D array");
         }
         Vector2u _size = Vector2u(bufsize[0].get<u_int32_t>(), bufsize[1].get<u_int32_t>());
         _buffer.resize(_size);

        auto buffer = parser.key_value("buffer");
        if (!buffer.is_array() && buffer.size() == _size.x() * _size.y()) {
            throw std::runtime_error("Buffer is not an 2D array");
        }
        for (u_int32_t y = 0; y < _size.y(); y++) {
            for (u_int32_t x = 0; x < _size.x(); x++) {
                Pixel p = Pixel(buffer[y * _size.x() + x].get<u_int32_t>());
                if (p.color == 1) {
                    p = Pixel(255, 255, 255, 255);
                }
                _buffer.setElement(Vector2u(x,y), p);
            }
        }
    }

    Widget::~Widget()
    {
    }

    void Widget::draw(Display &d)
    {
        Transform t = getTransform();

        for (u_int32_t y = 0; y < _buffer.getSize().y(); y++) {
            for (u_int32_t x = 0; x < _buffer.getSize().x(); x++) {
                Pixel p = _buffer.getElement(Vector2u(x, y));
                if (GET_A(p.color) == 0) {
                    continue;
                }
                Vector2f pos = t.transformPoint(x, y);
                Vector2u posu = Vector2u(static_cast<u_int32_t>(pos.x()), static_cast<u_int32_t>(pos.y()));
                d.setPixel(posu, p);
            }
        }
    }
}