//
// Created by thomas on 17/05/24.
//

#include "TDL/Matrix/Transformation.hpp"
#include "TDL/Matrix/Transform.hpp"
#include <iomanip>
#include <iostream>



namespace tdl
{
    Transformable::Transformable() : m_origin(0, 0), m_position(0, 0), m_scale(1, 1), m_rotation(0), m_transform(), TransformNeedUpdate(true)
    {
    }

    Transformable::~Transformable()
    {
    }

    void Transformable::setPosition(const Vector2u &position)
    {
        setPosition(position.x(), position.y());
    }

    void Transformable::setPosition(u_int32_t x, u_int32_t y)
    {
        m_position.x() = x <= 1 ? 1 : x;
        m_position.y() = y <= 1 ? 1 : y;
        TransformNeedUpdate = true;
    }

    void Transformable::setRotation(double angle)
    {
        m_rotation = angle;
        TransformNeedUpdate = true;
    }

    void Transformable::setScale(const Vector2f &factors)
    {
        setScale(factors.x(), factors.y());
    }

    void Transformable::setScale(double factorX, double factorY)
    {
        m_scale.x() = factorX;
        m_scale.y() = factorY;
        TransformNeedUpdate = true;
    }

    void Transformable::setOrigin(const Vector2f &origin)
    {
        setOrigin(origin.x(), origin.y());
    }

    void Transformable::setOrigin(double x, double y)
    {
        m_origin.x() = x;
        m_origin.y() = y;
        TransformNeedUpdate = true;
    }

    const Vector2f &Transformable::getPosition() const
    {
        return m_position;
    }

    double Transformable::getRotation() const
    {
        return m_rotation;
    }

    const Vector2f &Transformable::getScale() const
    {
        return m_scale;
    }

    const Vector2f &Transformable::getOrigin() const
    {
        return m_origin;
    }

    void Transformable::move(const Vector2f &offset)
    {
        move(offset.x(), offset.y());
    }

    void Transformable::move(double offsetX, double offsetY)
    {
        m_position.x() = m_position.x() + offsetX;
        m_position.y() = m_position.y() + offsetY;
        TransformNeedUpdate = true;
    }

    void Transformable::rotate(double angle)
    {
        m_rotation += angle;
        TransformNeedUpdate = true;
    }

    void Transformable::scale(const Vector2f &factor)
    {
        scale(factor.x(), factor.y());
    }

    void Transformable::scale(double factorX, double factorY)
    {
        m_scale.x() = m_scale.x() * factorX;
        m_scale.y() = m_scale.y() * factorY;
        TransformNeedUpdate = true;
    }

    const Transform& Transformable::getTransform() const
    {
        if (TransformNeedUpdate)
        {
            double angle = -m_rotation * M_PI / 180.f;
            double cosine = std::cos(angle);
            double sine = std::sin(angle);
            double sxc = m_scale.x() * cosine;
            double syc = m_scale.y() * cosine;
            double sxs = m_scale.x() * sine;
            double sys = m_scale.y() * sine;
            double tx = -m_origin.x() * sxc - m_origin.y() * sys + m_position.x();
            double ty = m_origin.x() * sxs - m_origin.y() * syc + m_position.y();
            m_transform = Transform(sxc, sys, tx,
                                    -sxs, syc, ty,  
                                    0.f, 0.f, 1.f);
            TransformNeedUpdate = false;
        }
        return m_transform;
    }
}