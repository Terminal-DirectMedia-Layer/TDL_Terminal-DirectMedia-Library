
#ifndef TDL_TRANSFORM_HPP
    #define TDL_TRANSFORM_HPP

#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Math/UnitConverter.hpp"
#include "TDL/Utils/Math/Rect.hpp"

namespace tdl {
    /**
     * @class Transform
     * @brief Transform class for apply matrix transformation on transformable object
     */
    class Transform {
        public:

        /**
         * @brief Construct a new Transform object
         * default constructor
         * @note the default constructor will create an identity matrix
         * @overload
         */
        constexpr Transform() :
            m_matrix{1, 0, 0, 0, 1, 0, 0, 0, 1}
        {
        }

        /**
         * @brief Construct a new Transform object
         * @param a the first element of the matrix
         * @param b the second element of the matrix
         * @param c the third element of the matrix
         * @param d the fourth element of the matrix
         * @param e the fifth element of the matrix
         * @param f the sixth element of the matrix
         * @param g the seventh element of the matrix
         * @param h the eighth element of the matrix
         * @param i the ninth element of the matrix
         */
        constexpr Transform(float a, float b, float c, float d, float e, float f, float g, float h, float i) :
            m_matrix{a, b, c, d, e, f, g, h, i}
        {
        }

        /**
         * @brief Construct a new Transform object
         * @param matrix the matrix to copy
         */
        [[nodiscard]] constexpr const float *getMatrix() const
        {
            return m_matrix;
        }

        /**
         * @brief combine two transform
         * @param transform the transform to combine
         * @return Transform& the combined transform
         */
        constexpr Transform &combine(const Transform &transform)
        {
            const float *b = transform.m_matrix;
            const float *a = m_matrix;
            //clang-format off
            *this = Transform( a[0] * b[0] + a[1] * b[3] + a[2] * b[6],
                               a[0] * b[1] + a[1] * b[4] + a[2] * b[7],
                               a[0] * b[2] + a[1] * b[5] + a[2] * b[8],
                               a[3] * b[0] + a[4] * b[3] + a[5] * b[6],
                               a[3] * b[1] + a[4] * b[4] + a[5] * b[7],
                               a[3] * b[2] + a[4] * b[5] + a[5] * b[8],
                               a[6] * b[0] + a[7] * b[3] + a[8] * b[6],
                               a[6] * b[1] + a[7] * b[4] + a[8] * b[7],
                               a[6] * b[2] + a[7] * b[5] + a[8] * b[8]);
            //clang-format on
            return *this;
        }

        constexpr Transform getInverse() const {
            const float *m = m_matrix;
            float det = m[0] * (m[4] * m[8] - m[7] * m[5]) -
                        m[1] * (m[3] * m[8] - m[5] * m[6]) +
                        m[2] * (m[3] * m[7] - m[4] * m[6]);

            if (det != 0) {
                float invDet = 1.f / det;

                return Transform((m[4] * m[8] - m[7] * m[5]) * invDet,
                                 -(m[1] * m[8] - m[2] * m[7]) * invDet,
                                 (m[1] * m[5] - m[2] * m[4]) * invDet,
                                 -(m[3] * m[8] - m[5] * m[6]) * invDet,
                                 (m[0] * m[8] - m[2] * m[6]) * invDet,
                                 -(m[0] * m[5] - m[3] * m[2]) * invDet,
                                 (m[3] * m[7] - m[4] * m[6]) * invDet,
                                 -(m[0] * m[7] - m[1] * m[6]) * invDet,
                                 (m[0] * m[4] - m[1] * m[3]) * invDet);
            } else {
                return Transform(); // Return identity matrix if determinant is zero
            }
        }

        /**
         * @brief get the inverse of the transform
         * 
         * @param x the x position
         * @param y the y position
         * @return constexpr Vector2f the inverse of the transform
         */
        [[nodiscard]] constexpr Vector2f transformPoint(float x, float y) const
        {
            return {m_matrix[0] * x + m_matrix[1] * y + m_matrix[2],
                            m_matrix[3] * x + m_matrix[4] * y + m_matrix[5]};
        }

        /**
         * @brief get the inverse of the transform
         * 
         * @param point the point to transform
         * @return constexpr Vector2f the inverse of the transform
         */
        [[nodiscard]] constexpr Vector2f transformPoint(const Vector2f &point) const
        {
            return transformPoint(point.x(), point.y());
        }

        /**
         * @brief get the inverse of the transform
         * 
         * @param rectangle the rectangle to transform
         * @return constexpr RectF the inverse of the transform
         */
        [[nodiscard]] constexpr RectF transformRect(const RectF &rectangle) const
        {
            const float left = rectangle.x();
            const float right = rectangle.y();
            const float top = rectangle.height();
            const float bottom = rectangle.width();

            const Vector2f points[] = {transformPoint(left, top),
                                       transformPoint(right, top),
                                       transformPoint(left, bottom),
                                       transformPoint(right, bottom)};

            float minX = points[0].x();
            float maxX = points[0].x();
            float minY = points[0].y();
            float maxY = points[0].y();

            for (int i = 1; i < 4; ++i)
            {
                const float x = points[i].x();
                const float y = points[i].y();

                if (x < minX)
                    minX = x;
                else if (x > maxX)
                    maxX = x;

                if (y < minY)
                    minY = y;
                else if (y > maxY)
                    maxY = y;
            }

            return {minX, minY, maxX - minX, maxY - minY};
        }

        /**
         * @brief get the inverse of the transform
         * 
         * @param x the x position
         * @param y the y position
         * @return constexpr Transform& the inverse of the transform with the translation
         */
        constexpr Transform &translate(float x, float y)
        {
            // clang-format off
            const Transform transform(1.f, 0.f, x,
                                      0.f, 1.f, y,
                                      0.f, 0.f, 1.f);
            // clang-format on
            return combine(transform);
        }

        Vector2u getScaledSize(const Vector2u &originalSize) const {
            float scaledWidth = originalSize.x() * m_matrix[0];
            float scaledHeight = originalSize.y() * m_matrix[4];
            return Vector2u(static_cast<u_int32_t>(scaledWidth), static_cast<u_int32_t>(scaledHeight));
        }

        /**
         * @brief get the inverse of the transform
         * 
         * @param offset the offset to translate
         * @return constexpr Transform& the inverse of the transform with the rotation
         */
        constexpr Transform &rotate(float angle)
        {
            const float rad = UnitConverter<double>::toRadians(angle);
            const float s = std::sin(rad);
            const float c = std::cos(rad);
            // clang-format off
            const Transform transform(c, -s, 0.f,
                                      s, c, 0.f,
                                      0.f, 0.f, 1.f);
            // clang-format on
            return combine(transform);
        }

        /**
         * @brief set the tranform to rotate around a point
         * 
         * @param angle the angle to rotate
         * @param centerX the x position of the center
         * @param centerY the y position of the center
         * @return constexpr Transform& the inverse of the transform with the rotation
         * @overload
         */
        constexpr Transform &rotate(float angle, float centerX, float centerY)
        {
            translate(centerX, centerY);
            rotate(angle);
            return translate(-centerX, -centerY);
        }

        /**
         * @brief set the tranform to rotate around a point
         * 
         * @param angle the angle to rotate
         * @param center the center of the rotation
         * @return constexpr Transform& the inverse of the transform with the rotation
         */
        constexpr Transform &rotate(float angle, const Vector2f &center)
        {
            return rotate(angle, center.x(), center.y());
        }

        /**
         * @brief set the tranform to scale
         * 
         * @param scaleX the scale on the x axis
         * @param scaleY the scale on the y axis
         * @return constexpr Transform& the inverse of the transform with the scale
         */
        constexpr Transform &scale(float scaleX, float scaleY)
        {
            // clang-format off
            const Transform transform(scaleX, 0.f, 0.f,
                                      0.f, scaleY, 0.f,
                                      0.f, 0.f, 1.f);
            // clang-format on
            return combine(transform);
        }

        /**
         * @brief set the tranform to scale
         * 
         * @param factors the scale factors
         * @return constexpr Transform& the inverse of the transform with the scale
         * @overload
         */
        constexpr Transform &scale(const Vector2f &factors)
        {
            return scale(factors.x(), factors.y());
        }

    private:
        float m_matrix[9]; /* !< the matrix of the transform */
    };

    /**
     * @brief operator * for transform
     * 
     * @param left the left transform
     * @param right the right transform
     * @return constexpr Transform the combined transform
     */
    constexpr Transform operator * (const Transform &left, const Transform &right)
    {
        Transform result = left;
        return result.combine(right);
    }

    /**
     * @brief operator *= for transform
     * 
     * @param left the left transform
     * @param right the right transform
     * @return constexpr Transform& the combined transform
     */
    constexpr Transform &operator *= (Transform &left, const Transform &right)
    {
        return left.combine(right);
    }

    /**
     * @brief operator * for transform
     * 
     * @param left the left transform
     * @param right the right vector
     * @return constexpr Vector2f the transformed vector
     */
    constexpr Vector2f operator * (const Transform &left, const Vector2f &right)
    {
        return left.transformPoint(right);
    }

    /**
     * @brief operator *= for transform
     * 
     * @param left the left transform
     * @param right the right vector
     * @return constexpr Vector2f the transformed vector
     */
    constexpr Vector2f operator *= (const Transform &right, const Vector2f &left)
    {
        return right.transformPoint(left);
    }

    /**
     * @brief operator * for transform
     * 
     * @param left the left transform
     * @param right the right matrix
     * @return constexpr PixelMatrix the transformed matrix
     */
    //PixelMatrix operator *(const Transform &left, PixelMatrix &right);

    /**
     * @brief operator *= for transform
     * 
     * @param left the left transform
     * @param right the right matrix
     * @return constexpr PixelMatrix the transformed matrix
     */
    constexpr bool operator == (const Transform &left, const Transform &right)
    {
        const float *a = left.getMatrix();
        const float *b = right.getMatrix();
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] &&
               a[3] == b[3] && a[4] == b[4] && a[5] == b[5] &&
               a[6] == b[6] && a[7] == b[7] && a[8] == b[8];
    }

    /**
     * @brief operator *= for transform
     * 
     * @param left the left transform
     * @param right the right matrix
     * @return constexpr PixelMatrix the transformed matrix
     */
    constexpr bool operator != (const Transform &left, const Transform &right)
    {
        return !(left == right);
    }
}
#endif //TDL_TRANSFORM_HPP
