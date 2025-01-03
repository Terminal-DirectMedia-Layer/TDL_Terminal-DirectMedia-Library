/**
* @file Placeable.hpp
* @brief Header file for the Positionable class.
*/
#ifndef POSITIONABLE_HPP
    #define POSITIONABLE_HPP

#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Matrix/Transform.hpp"

    namespace tdl
    {
        /**
        * @class Placeable
        * @brief A class that represents a placeable object.
        * Its one of the feature of the FrameBuffer class.
        */
        class Placeable
        {
        public:

            /**
            * @brief Constructor for the Placeable class.
            */
            Placeable();

            /**
            * @brief Destructor for the Placeable class.
            */
            virtual ~Placeable();

            /**
            * @brief sets the position of the object.
            * @param position The position of the object.
            */
            void setPosition(const Vector2u &position);

            /**
            * @overload
            * @brief sets the position of the object.
            * @param x The x position of the object.
            * @param y The y position of the object.
            */
            void setPosition(u_int32_t x, u_int32_t y);

            /**
            * @brief gets the position of the object.
            * @return The position of the object.
            */
            const Vector2u &getPosition() const;

            /**
            * @brief moves the object by a given offset.
            * @param offset The offset to move the object by.
            */
            void move(const Vector2u &offset);

            /**
            * @overload
            * @brief moves the object by a given offset.
            * @param offsetX The x offset to move the object by.
            * @param offsetY The y offset to move the object by.
            */
            void move(u_int32_t offsetX, u_int32_t offsetY);

            /**
            * @brief gets the transformation of the object.
            * @return The transformation matrix of the object.
            */
            const Transform &getTransform() const;

        protected:
            Vector2u m_position; /**< The position of the object. */
            mutable Transform m_transform; /**< The transformation matrix of the object. */
            mutable bool TransformNeedUpdate; /**< A flag to check if the transformation matrix needs to be updated. */
        };
    }

#endif //POSITIONABLE_HPP
