
#ifndef ADRAWABLE_HPP
    #define ADRAWABLE_HPP

#include "TDL/Graphics/Drawable/IDrawable.hpp"
#include "TDL/Utils/Matrix/Matrix.hpp"
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"

namespace tdl {

    class ADrawable : public IDrawable {
        public:
            virtual void draw(Window *d) = 0;

            protected:
                ADrawable(Vector2u size);
                ~ADrawable();

                Matrix<Pixel> _matrix;
    };
};

#endif //ADRAWABLE_HPP
