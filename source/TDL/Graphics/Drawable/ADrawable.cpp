
#include "TDL/Graphics/Drawable/ADrawable.hpp"
#include "TDL/Graphics/Window/Window.hpp"
#include "TDL/Utils/Memory/Arena.hpp"

namespace tdl {
    ADrawable::ADrawable(Vector2u size) 
    {
        _matrix = Matrix<Pixel>(size);
    }

    ADrawable::~ADrawable()
    {
    }

} // namespace tdl