#ifndef TDL_Matrix_HPP
    #define TDL_Matrix_HPP

#include <TDL/Utils/Math/Rect.hpp>
#include <TDL/Utils/Math/Vector.hpp>
#include <iostream>
#include <optional>
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include <vector>

namespace tdl {

    /**
     * @class Matrix
     * @brief Matrix class
     * this class is a matrix of T
     * and its implement the basic function to manipulate the matrix
     */
template <typename T>
class Matrix {
    public:

        /**
         * @brief Construct a new Matrix:: Matrix object
         * default class constructor it will be create an new matrix with 0 of size
         */
        Matrix() : _size(0, 0) {
        }

        /**
         * @overload
         * @brief Construct a new Matrix:: Matrix object
         * it will be create an size lenght matrix with black pixel
         *
         * @param size the size of the matrix
         */
        Matrix(Vector2u size) : _size(std::move(size)) {
            _matrix = std::vector<T>(_size.x() * _size.y());
        }

        /**
         * @brief Destroy the Matrix:: Matrix object
         *
         */
        ~Matrix() {}

        /**
         * @brief sets a pixel at a given position.
         * @param pos The position of the pixel.
         * @param element The element to set.
         */
    inline void setElement(const Vector2u &pos, T element) {
            if (pos.x() < _size.x() && pos.y() < _size.y()) {
                _matrix[pos.y() * _size.x() + pos.x()] = element;
            }
        }

        /**
         * @overload
         * @brief sets a pixel at a given position.
         * @param x The x position of the pixel.
         * @param y The y position of the pixel.
         * @param element The element to set.
         */

    inline void setElement(u_int32_t x, u_int32_t y, T element) {
            if (x < _size.x() && y < _size.y()) {
                _matrix[y * _size.x() + x] = element;
            }
        }

    inline void setElement(Vector2f pos, T element) {
            if (pos.x() < _size.x() && pos.y() < _size.y()) {
                _matrix[static_cast<u_int32_t>(pos.y()) * _size.x() + static_cast<u_int32_t>(pos.x())] = element;
            }
        }

        /**
         * @brief get the element at the position pos
         * @param pos the position of the element
         * @return T the element at the position pos
         */
        inline T &getElement(const Vector2u &pos) {
            if (pos.x() <= _size.x() && pos.y() <= _size.y()) {
                return _matrix[pos.y() * _size.x() + pos.x()];
            }
            return _matrix[0];
        }

        /**
         * @overload
         * @brief gets a pixel at a given position.
         * @param x The x position of the pixel.
         * @param y The y position of the pixel.
         * @return The element at the given position.
         */
        inline T &getElement(u_int32_t x, u_int32_t y) {
            if (x <= _size.x() && y <= _size.y()) {
                return _matrix[y * _size.x() + x];
            }
            return _matrix[0];
        }

        inline T &getElement(Vector2i pos) {
            if (pos.x() <= _size.x() && pos.y() <= _size.y()) {
                return _matrix[pos.y() * _size.x() + pos.x()];
            }
            return _matrix[0];
        }

        /**
         * @brief append the matrix of element to the matrix
         *
         * @param elements the matrix of element to append
         */
        void append(std::vector<T> &elements) {
            _matrix.insert(_matrix.end(), elements.begin(), elements.end());
            _size = Vector2u(_size.x(), _size.y() + elements.size());
        }

        /**
         * @brief resize the matrix to the size
         *
         * @param size the new size of the matrix
         */
        void resize(Vector2u &size) {
            _size = size;
            _matrix.resize(size.x() * size.y());
        }

        /**
         * @brief reset the matrix
         * @note in opposite of clear it will reset the size of the matrix to 0
         */
        void reset() {
            _matrix.clear();
            _size = Vector2u(0, 0);
        }

        /**
         * @brief clear the matrix with black pixel
         *
         */
        void clear() {
            _matrix.clear();
            _matrix = std::vector<T>(_size.x() * _size.y());
        }

        /**
         * @brief get the size of the matrix
         * @return Vector2u the size of the matrix
         */
        [[nodiscard]] inline Vector2u getSize() const {
            return _size;
        }

        /**
         * @brief Get the matrix object
         *
         * @return T* C style correspond of the matrix of element
         */
        inline T* getRawData() {
            return _matrix.data();
        }

        inline bool empty() {
            return _matrix.empty();
        }

        inline void fill(T element) {
            std::fill(_matrix.begin(), _matrix.end(), element);
        }

        /**
         * @brief operator- to crop the matrix
         *
         * @param crop the crop to apply
         * @return Matrix the new matrix
         */
        Matrix operator-(RectU crop)
        {
            Matrix newMatrix = Matrix(Vector2u (crop.width(), crop.height()));
            for (u_int32_t y = 0; y < crop.height(); y++) {
                for (u_int32_t x = 0; x < crop.width(); x++) {
                    T element = this->getElement(Vector2u(x + crop.x(), y + crop.y()));
                    newMatrix.setElement(Vector2u(x, y), element);
                }
            }
            return newMatrix;
        }

        /**
         * @brief operator== to compare two matrix
         *
         * @param other the other matrix to compare
         * @return bool true if the matrix are equals
         */
        bool  operator==(const Matrix &other) const {
            return _size == other._size && _matrix == other._matrix;
        }

    private:
        Vector2u _size; /*!< the size of the matrix */
        std::vector<T> _matrix; /*!< the matrix */
    };
}

#endif //TDL_Matrix_HPP