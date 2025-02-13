
#ifndef IDRAWABLE_HPP
    #define IDRAWABLE_HPP

#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Utils/Math/Vector.hpp"

namespace tdl {

        class Window;

        class IDrawable {
            public:
                virtual void draw(Window *d) = 0;
                virtual bool getPixelAtPos(Vector2i pos, Pixel &pixel) = 0;
              virtual Vector2u getPosition() = 0;
                virtual Vector2u getSize() = 0;
        };
};

#endif //IDRAWABLE_HPP
