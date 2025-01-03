
#include "TDL/Graphics/Drawable/ADrawable.hpp"
#include "TDL/Graphics/Window/Window.hpp"

namespace tdl {
    ADrawable::ADrawable(Vector2u size) : _matrix(size)
    {
    }

    ADrawable::~ADrawable()
    {
    }

} // namespace tdl