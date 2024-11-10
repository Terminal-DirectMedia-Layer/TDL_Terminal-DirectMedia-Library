#ifndef TDL_Matrix_HPP
    #define TDL_Matrix_HPP

#include <vector>
#include <TDL/Utils/Math/Vector.hpp>
#include <optional>
#include <TDL/Utils/Math/Rect.hpp>

namespace tdl {

template <typename T>
class Matrix {
    public:
        Matrix() : _size(0, 0) {
        }

        Matrix(Vector2u size) : _size(std::move(size)) {
            _matrix = std::vector<T>(_size.x() * _size.y());
        }

        ~Matrix() {}

        void setElement(const Vector2u &pos, T &element) {
            if (pos.x() < _size.x() && pos.y() < _size.y()) {
                _matrix[pos.y() * _size.x() + pos.x()] = element;
            }
        }

        void setElement(u_int32_t x, u_int32_t y, T &element) {
            if (x < _size.x() && y < _size.y()) {
                _matrix[y * _size.x() + x] = element;
            }
        }

        T getElement(const Vector2u &pos) {
            if (pos.x() < _size.x() && pos.y() < _size.y()) {
                return _matrix[pos.y() * _size.x() + pos.x()];
            }
            return T();
        }

        T getElement(u_int32_t x, u_int32_t y) {
            if (x < _size.x() && y < _size.y()) {
                return _matrix[y * _size.x() + x];
            }
            return T();
        }

        void append(std::vector<T> &elements) {
            _matrix.insert(_matrix.end(), elements.begin(), elements.end());
            _size = Vector2u(_size.x(), _size.y() + elements.size());
        }

        void resize(Vector2u &size) {
            _size = size;
            _matrix.resize(size.x() * size.y());
        }

        void reset() {
            _matrix.clear();
            _size = Vector2u(0, 0);
        }

        void clear() {
            _matrix.clear();
            _matrix = std::vector<T>(_size.x() * _size.y());
        }

        [[nodiscard]] inline Vector2u getSize() const {
            return _size;
        }

        inline T* getRawData() {
            return _matrix.data();
        }

        Matrix operator-(std::optional<RectU> crop)
        {
            if (!crop.has_value())
                return *this;
            Matrix newMatrix = Matrix(Vector2u (crop.value().width(), crop.value().height()));
            for (u_int32_t y = 0; y < crop.value().height(); y++) {
                for (u_int32_t x = 0; x < crop.value().width(); x++) {
                    newMatrix.setElement(Vector2u(x, y), getElement(Vector2u(x + crop.value().x(), y + crop.value().y())));
                }
            }
            return newMatrix;
        }

        bool  operator==(const Matrix &other) const {
            return _size == other._size && _matrix == other._matrix;
        }

    private:
        Vector2u _size; /*!< the size of the matrix */
        std::vector<T> _matrix; /*!< the matrix */
    };
}

#endif //TDL_Matrix_HPP