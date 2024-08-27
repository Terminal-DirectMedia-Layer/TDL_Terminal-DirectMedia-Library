
#ifndef LIBNCURSE_RECT_HPP
    #define LIBNCURSE_RECT_HPP


#include <tuple>
#include <cstdint>
namespace tdl {

    /**
     * @struct Rect
     * @brief Rect struct
     * 
     * @tparam T the type of the rect
     */
    template<typename T>
    struct Rect {
        std::tuple<T, T, T, T> _values; /* !< the values of the rect */

    /**
     * @brief Construct a new Rect object
     * 
     */
        constexpr Rect() : _values(0, 0, 0, 0) {}

    /**
     * @brief Construct a new Rect object
     * @param x the x value
     * @param y the y value
     * @param width the width value
     * @param height the height value
     * @overload
     */
        constexpr Rect(T x, T y, T width, T height)  : _values(x, y, width, height) {}

    /**
     * @brief Construct a new Rect object
     * @param v the rect to copy
     * @overload
     */
        constexpr Rect(const Rect &v) : _values(v.x(), v.y(), v.width(), v.height()) {}

    /**
     * @brief Construct a new Rect object
     * 
     * @param v the rect to move
     * @overload
     */
        constexpr Rect(Rect &&v)  noexcept : _values(v.x(), v.y(), v.width(), v.height()) {}

    /**
     * @brief operator=
     * 
     * @param v  the rect to copy
     * @return constexpr Rect& the rect copied
     */
        constexpr Rect &operator=(const Rect &v)
        {
            this->x() = v.x();
            this->y() = v.y();
            this->width() = v.width();
            this->height() = v.height();
            return *this;
        }

    /**
     * @brief operator=
     * 
     * @param v the rect to move
     * @return constexpr Rect&
     */
        constexpr Rect &operator=(Rect &&v) noexcept
        {
            this->x() = v.x();
            this->y() = v.y();
            this->width() = v.width();
            this->height() = v.height();
            return *this;
        }

    /**
     * @brief operator+
     * 
     * @param v the rect to add
     * @return Rect the new rect
     */
        Rect operator+(const Rect &v) const
        {
            return {this->x() + v.x(), this->y() + v.y(), this->width() + v.width(), this->height() + v.height()};
        }

    /**
     * @brief operator-
     *
     * @param v the rect to substract
     * @return Rect the new rect
     */
        Rect operator-(const Rect &v) const
        {
            return {this->x() - v.x(), this->y() - v.y(), this->width() - v.width(), this->height() - v.height()};
        }

    /**
     * @brief operator*
     * 
     * @param v the rect to multiply
     * @return Rect the new rect
     */
        Rect operator*(const Rect &v) const
        {
            return {this->x() * v.x(), this->y() * v.y(), this->width() * v.width(), this->height() * v.height()};
        }

    /**
     * @brief operator/
     * 
     * @param v the rect to divide
     * @return Rect the new rect
     */
        Rect operator/(const Rect &v) const
        {
            return {this->x() / v.x(), this->y() / v.y(), this->width() / v.width(), this->height() / v.height()};
        }

    /**
     * @brief operator+=
     * 
     * @param v the rect to add
     * @return Rect the new rect
     */
        Rect &operator +=(const Rect &v)
        {
            this->x() += v.x();
            this->y() += v.y();
            this->width() += v.width();
            this->height() += v.height();
            return *this;
        }

    /**
     * @brief operator-=
     * 
     * @param v the rect to substract
     * @return Rect the new rect
     */
        Rect &operator -=(const Rect &v)
        {
            this->x() -= v.x();
            this->y() -= v.y();
            this->width() -= v.width();
            this->height() -= v.height();
            return *this;
        }

    /**
     * @brief operator*=
     * 
     * @param v the rect to multiply
     * @return Rect the new rect
     */
        Rect &operator *=(const Rect &v)
        {
            this->x() *= v.x();
            this->y() *= v.y();
            this->width() *= v.width();
            this->height() *= v.height();
            return *this;
        }

    /**
     * @brief operator/=
     * 
     * @param v the rect to divide
     * @return Rect the new rect
     */
        Rect &operator /=(const Rect &v)
        {
            this->x() /= v.x();
            this->y() /= v.y();
            this->width() /= v.width();
            this->height() /= v.height();
            return *this;
        }

    /**
     * @brief operator==
     * 
     * @param v the rect to compare
     * @return true if the rect are equals
     * @return false if the rect are not equals
     */
        bool operator==(const Rect &v) const
        {
            return this->x() == v.x() && this->y() == v.y() && this->width() == v.width() && this->height() == v.height();
        }

    /**
     * @brief operator!=
     * 
     * @param v the rect to compare
     * @return true if the rect are not equals
     * @return false if the rect are equals
     */
        bool operator!=(const Rect &v) const
        {
            return !(*this == v);
        }

    /**
     * @brief get the x value
     * 
     * @return constexpr T&
     */
        constexpr T &x() { return std::get<0>(_values); }

    /**
     * @brief get the y value
     * 
     * @return constexpr T& 
     */
        constexpr T &y() { return std::get<1>(_values); }

    /**
     * @brief get the width value
     * 
     * @return constexpr T& 
     */
        constexpr T &width() { return std::get<2>(_values); }

    /**
     * @brief get the height value
     * 
     * @return constexpr T& 
     */
        constexpr T &height() { return std::get<3>(_values); }

    /**
     * @brief get the x value
     * 
     * @return constexpr T 
     */
        constexpr T x() const { return std::get<0>(_values); }

    /**
     * @brief get the y value
     * 
     * @return constexpr T 
     */
        constexpr T y() const { return std::get<1>(_values); }

    /**
     * @brief get the width value
     * 
     * @return constexpr T 
     */
        constexpr T width() const { return std::get<2>(_values); }

    /**
     * @brief get the height value
     * 
     * @return constexpr T 
     */
        constexpr T height() const { return std::get<3>(_values); }
    };

    using RectI = Rect<int32_t>; /* !< the rect with int32_t */
    using RectU = Rect<u_int32_t>; /* !< the rect with u_int32_t */
    using RectF = Rect<double>; /* !< the rect with double */
}
#endif //LIBNCURSE_RECT_HPP
    