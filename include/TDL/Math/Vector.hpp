
#ifndef TDL_VECTOR_HPP
    #define TDL_VECTOR_HPP

#include <tuple>
#include <cstdint>
#include <variant>
#include <cmath>

namespace tdl
{

/**
 * @struct Vector2
 * @brief Vector template for 2D vector
 * The vector was composed like this (x, y)
 * @tparam T the type of the vector
 */
    template<typename T>
    struct Vector2
    {
        std::tuple<T, T> _values; /* !< the values of the vector */

    /**
    * @brief Construct a new Vector2 object
    * 
    */
        constexpr Vector2() : _values(0, 0) {}

    /**
     * @brief Construct a new Vector2 object
     * 
     */
        constexpr Vector2(T x, T y)  : _values(x, y) {}

    /**
     * @brief Construct a new Vector2 object
     * 
     */
        constexpr Vector2(const Vector2 &v) : _values(v.x(), v.y()) {}

    /**
     * @brief Construct a new Vector2 object
     * 
     */
        constexpr Vector2(Vector2 &&v)  noexcept : _values(v.x(), v.y()) {}

    /**
     * @brief operator=
     * 
     * @param v the vector to copy
     * @return Vector2&
     */
        Vector2 &operator=(const Vector2 &v)
        {
            this->x() = v.x();
            this->y() = v.y();
            return *this;
        }

    /**
     * @brief operator=
     * 
     * @param v the vector to move
     * @return Vector2& 
     */
        Vector2 &operator=(Vector2 &&v) noexcept
        {
            this->x() = v.x();
            this->y() = v.y();
            return *this;
        }

    /**
     * @brief operator+
     * 
     * @param v the vector to add
     * @return Vector2 
     */
        Vector2 operator+(const Vector2 &v) const
        {
            return {this->x() + v.x(), this->y() + v.y()};
        }

    /**
     * @brief operator-
     * 
     * @param v the vector to substract
     * @return Vector2 
     */
        Vector2 operator-(const Vector2 &v) const
        {
            return {this->x() - v.x(), this->y() - v.y()};
        }

    /**
     * @brief operator*
     * 
     * @param v the vector to multiply
     * @return Vector2 
     */
        Vector2 operator*(const Vector2 &v) const
        {
            return {this->x() * v.x(), this->y() * v.y()};
        }

    /**
     * @brief operator/
     * 
     * @param v the vector to divide
     * @return Vector2 
     */
        Vector2 operator/(const Vector2 &v) const
        {
            return {this->x() / v.x(), this->y() / v.y()};
        }

    /**
     * @brief operator+=
     * 
     * @param v the vector to add
     * @return Vector2& 
     */
        Vector2 &operator+=(const Vector2 &v)
        {
            return *this = *this + v;
        }

    /**
     * @brief operator-=
     * 
     * @param v the vector to substract
     * @return Vector2& 
     */
        Vector2 &operator-=(const Vector2 &v)
        {
            return *this = *this - v;
        }

    /**
     * @brief operator*=
     * 
     * @param v the vector to multiply
     * @return Vector2& 
     */
        Vector2 &operator*=(const Vector2 &v)
        {
            return *this = *this * v;
        }

    /**
     * @brief operator/=
     * 
     * @param v the vector to divide
     * @return Vector2& 
     */
        Vector2 &operator/=(const Vector2 &v)
        {
            return *this = *this / v;
        }

    /**
     * @brief operator==
     * 
     * @param v the vector to compare
     * @return true if the vector are equals
     * @return false if the vector are not equals
     */
        bool operator==(const Vector2 &v) const
        {
            return this->x() == v.x() && this->y() == v.y();
        }

    /**
     * @brief operator!=
     * 
     * @param v the vector to compare
     * @return true if the vector are not equals
     * @return false if the vector are equals
     */
        bool operator!=(const Vector2 &v) const
        {
            return this->x() != v.x() || this->y() != v.y();
        }

    /**
     * @brief dot product of the vector
     * 
     * @param v the vector to dot
     * @return T the dot product of the vector
     */
        T dot(const Vector2 &v) const
        {
            return this->x() * v.x() + this->y() * v.y();
        }

    /**
     * @brief cross product of the vector
     * 
     * @param v the vector to cross
     * @return T the cross product of the vector
     */
        T cross(const Vector2 &v) const
        {
            return this->x() * v.y() - this->y() * v.x();
        }

    /**
     * @brief check if the vector inferiors or equals to the vector v
     * 
     * @param v the vector to compare
     * @return true if the vector is inferiors or equals to the vector v
     * @return false if the vector is not inferiors or equals to the vector v
     */
        bool operator <=(const Vector2 &v) const
        {
            return this->x() <= v.x() && this->y() <= v.y();
        }

    /**
     * @brief check if the vector superiors or equals to the vector v
     * 
     * @param v the vector to compar
     * @return true if the vector is superiors or equals to the vector v
     * @return false if the vector is not superiors or equals to the vector v
     */
        bool operator >=(const Vector2 &v) const
        {
            return this->x() >= v.x() && this->y() >= v.y();
        }

    public:

/**
 * @brief getter for x value of the vetor
 *
 * @tparam T the type of the vector
 * @param v the vector
 * @return T& the x value of the vector
 */
        constexpr T& x() { return std::get<0>(_values); }

/**
 * @brief getter for y value of the vetor
 *
 * @tparam T the type of the vector
 * @param v the vector
 * @return T& the y value of the vector
 */
        constexpr T& y() { return std::get<1>(_values); }

/**
 * @brief getter for x value of the vetor
 * 
 * @return constexpr T 
 */
        constexpr T y() const { return std::get<1>(_values); }

/**
 * @brief getter for y value of the vetor
 * 
 * @return constexpr T 
 */
        constexpr T x() const { return std::get<0>(_values); }
    };

/**
 * @brief Vector2i is a 2D vector with int32_t type
 * The vector was composed like this (x, y)
 */
    using Vector2i = Vector2<int32_t>;
    /**
     * @brief Vector2u is a 2D vector with u_int32_t type
     * The vector was composed like this (x, y)
     */
    using Vector2u = Vector2<u_int32_t>;

    /**
     * @brief Vector2f is a 2D vector with double type
     * The vector was composed like this (x, y)
     */
    using Vector2f = Vector2<double>;
}

#endif //TDL_VECTOR_HPP
