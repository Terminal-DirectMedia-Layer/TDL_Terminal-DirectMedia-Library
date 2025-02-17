
#include <iomanip>
#include <iostream>

#include "TDL/Graphics/FrameBuffer/Feature/Placeable.hpp"
#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Matrix/Transform.hpp"

namespace tdl
{
    Placeable::Placeable() : m_position(0, 0)
    {
    }

    Placeable::~Placeable()
    {
    }

    void Placeable::setPosition(const Vector2i &position)
    {
        setPosition(position.x(), position.y());
    }

    void Placeable::setPosition(int32_t x, int32_t y)
    {
        m_position.x() = x <= 1 ? 1 : x;
        m_position.y() = y <= 1 ? 1 : y;
        TransformNeedUpdate = true;
    }

    const Vector2i &Placeable::getPosition() const
    {
        return m_position;
    }

    void Placeable::move(const Vector2i &offset)
    {
        move(offset.x(), offset.y());
    }

    void Placeable::move(int32_t offsetX, int32_t offsetY)
    {
        m_position.x() += offsetX;
        m_position.y() += offsetY;
        TransformNeedUpdate = true;
    }

    const Transform &Placeable::getTransform() const
    {
        if (TransformNeedUpdate)
        {
            m_transform = Transform(1.f, 0.f, m_position.x(),
                                    0.f, 1.f, m_position.y(),
                                    0.f, 0.f, 1.f);
            TransformNeedUpdate = false;
        }
        return m_transform;
    }
}