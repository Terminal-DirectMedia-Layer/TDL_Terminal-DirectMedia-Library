/**
* @file Positionable.hpp
* @brief Header file for the Positionable class.
*/
#ifndef POSITIONABLE_HPP
    #define POSITIONABLE_HPP

#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Matrix/Transform.hpp"

    namespace tdl
    {
        class Positionable
        {
        public:
            Positionable();
            virtual ~Positionable();

            void setPosition(const Vector2u &position);
            void setPosition(u_int32_t x, u_int32_t y);

            const Vector2u &getPosition() const;

            void move(const Vector2u &offset);
            void move(u_int32_t offsetX, u_int32_t offsetY);

            const Transform &getTransform() const;

        protected:
            Vector2u m_position;
            mutable Transform m_transform;
            mutable bool TransformNeedUpdate;
        };
    }

#endif //POSITIONABLE_HPP
