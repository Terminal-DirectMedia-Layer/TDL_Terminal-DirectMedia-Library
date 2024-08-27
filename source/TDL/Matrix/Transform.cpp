
#include "TDL/Matrix/Transform.hpp"
#include "TDL/Math/UnitConverter.hpp"

namespace tdl
{
    PixelMatrix operator*(const Transform &left, PixelMatrix &right) {
        Vector2f topLeft = left.transformPoint(0, 0);
        Vector2f topRight = left.transformPoint(right.getSize().x(), 0);
        Vector2f bottomLeft = left.transformPoint(0, right.getSize().y());
        Vector2f bottomRight = left.transformPoint(right.getSize().x(), right.getSize().y());

        float minX = std::min({topLeft.x(), topRight.x(), bottomLeft.x(), bottomRight.x()});
        float maxX = std::max({topLeft.x(), topRight.x(), bottomLeft.x(), bottomRight.x()});
        float minY = std::min({topLeft.y(), topRight.y(), bottomLeft.y(), bottomRight.y()});
        float maxY = std::max({topLeft.y(), topRight.y(), bottomLeft.y(), bottomRight.y()});

        Vector2u newSize(static_cast<unsigned int>(maxX - minX), static_cast<unsigned int>(maxY - minY));
        PixelMatrix result(newSize);
        for (unsigned int y = 0; y < right.getSize().y(); y++) {
            for (unsigned int x = 0; x < right.getSize().x(); x++) {
                Vector2f point = left.transformPoint(x, y);
                if (point.x() >= 0 && point.x() < right.getSize().x() && point.y() >= 0 &&
                    point.y() < right.getSize().y()) {
                    result.setPixel(Vector2u(point.x(), point.y()),
                                    right.getPixel(Vector2u(x, y)));
                }
            }
        }
        return result;
    }

}