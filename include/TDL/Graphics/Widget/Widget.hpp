
#ifndef WIDGET_HPP
    #define WIDGET_HPP

#include <string>

#include "TDL/Graphics/Display/Display.hpp"
#include "TDL/Graphics/FrameBuffer/Feature/Transformation.hpp"
#include "TDL/Utils/Matrix/Matrix.hpp"

namespace tdl
{
    class Widget: public Transformable
    {
    public:
        Widget(std::string name, std::string file, Vector2u pos = Vector2u(0,0));
        ~Widget();
        void draw(Display &d);

        Vector2u getSize() const { return _buffer.getSize(); }
        std::string getName() const { return _name; }
        Matrix<Pixel> &getBuffer() { return _buffer; }

        void setNextPos(Vector2u pos) { _nextPos = pos; }
        Vector2u getNextPos() const { return _nextPos; }

        bool getPixelAtPos(Vector2i pos, Pixel &pixel) {
            if (pos.x() < 0 || pos.y() < 0 || pos.x() >= _buffer.getSize().x() || pos.y() >= _buffer.getSize().y())
                return false;
            pixel = _buffer.getElement(Vector2u(pos.x(), pos.y()));
            return true;
        }

    private:
        std::string _name;
        Vector2u _size;
        Matrix<Pixel> _buffer;
        Vector2u _nextPos;
    };
}


#endif //WIDGET_HPP
