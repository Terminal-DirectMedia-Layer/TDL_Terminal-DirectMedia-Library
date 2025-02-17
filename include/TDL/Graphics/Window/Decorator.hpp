
#ifndef DECORATOR_HPP
    #define DECORATOR_HPP

#include "TDL/Utils/Matrix/Matrix.hpp"
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Math/Rect.hpp"

namespace tdl
{
    enum class ClickRegion { NONE, TITLE_BAR, CLOSE_BUTTON, RESIZE_LEFT, RESIZE_RIGHT, RESIZE_BOTOM, BOTTOM_LEFT, BOTTOM_RIGHT, INSIDE, OUTSIDE };

    class Decorator
    {
        protected:
            Decorator();
            ~Decorator();
            Pixel *decGetRawData();

            void setDimensions(RectI dimensions);
            RectI getDimensions() const { return _dimensions; }

        private:
            Matrix<Pixel> _matrix;
            RectI _dimensions;
            int _tileSize;
            bool isResizeable;
    };
}
#endif //DECORATOR_HPP
