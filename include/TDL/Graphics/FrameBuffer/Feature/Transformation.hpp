
#ifndef TDL_TRANSFORMATION_HPP
    #define TDL_TRANSFORMATION_HPP

#include "TDL/Math/Vector.hpp"
#include "TDL/Matrix/Transform.hpp"

    namespace tdl
    {
        class Transformable
        {
        public:
            Transformable();
            virtual ~Transformable();

            void setPosition(const Vector2u &position);
            void setPosition(u_int32_t x, u_int32_t y);

            void setRotation(double angle);
            void setScale(const Vector2f &factors);
            void setScale(double factorX, double factorY);
            void setOrigin(const Vector2f &origin);
            void setOrigin(double x, double y);

            const Vector2f &getPosition() const;
            double getRotation() const;
            const Vector2f &getScale() const;
            const Vector2f &getOrigin() const;

            void move(const Vector2f &offset);
            void move(double offsetX, double offsetY);
            void rotate(double angle);
            void scale(const Vector2f &factor);
            void scale(double factorX, double factorY);

            const Transform &getTransform() const;

        protected:
            Vector2f m_origin;
            Vector2f m_position;
            Vector2f m_scale;
            double m_rotation;
            mutable Transform m_transform;
            mutable bool TransformNeedUpdate;
        };
    }

#endif //TDL_TRANSFORMATION_HPP
