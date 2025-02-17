
#include "TDL/Graphics/Window/Decorator.hpp"

namespace tdl
{
    Decorator::Decorator()
    {
    }

    Decorator::~Decorator()
    {
    }

    Pixel *Decorator::decGetRawData()
    {
        return _matrix.getRawData();
    }

    void Decorator::setDimensions(RectI dimensions)
    {
        _dimensions = dimensions;
        Vector2u dim = Vector2u(dimensions.width(), dimensions.height());
        _matrix.resize(dim);
        _matrix.fill(Pixel(255, 255, 255, 255));
    }
} // namespace tdl