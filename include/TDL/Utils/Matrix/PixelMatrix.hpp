
#ifndef TDLPixelMatrix_HPP
    #define TDLPixelMatrix_HPP
#include <vector>
#include <iostream>
#include <optional>
#include <queue>
#include <map>
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Math/Vector.hpp"
#include "TDL/Math/Rect.hpp"

namespace tdl {

/**
 * @struct CharColor
 * @brief the CharColor structure
 * it permited to store the color of a char
 * and the shape of the char
 * @note the shape is a string of 6 char
 */
    struct CharColor {
        const char *shape{};
        Pixel ForeGround;
        Pixel BackGround;
    };

/**
 * @class PixelMatrix
 * @brief PixelMatrix class
 * this class is a matrix of pixel
 * and its implement the basic function to manipulate the matrix
 */
    class PixelMatrix {
        public:

/**
 * @brief Construct a new Pixel Matrix:: Pixel Matrix object
 * default class constructor it will be create an new matrix with 0 of size
 */
            PixelMatrix();

/**
* @brief Construct a new Pixel Matrix:: Pixel Matrix object
* it will be create an size lenght matrix with black pixel
*
* @param size the size of the matrix
*/
            explicit PixelMatrix(Vector2u size);

/**
 * @brief Destroy the Pixel Matrix:: Pixel Matrix object
 *
 */
            ~PixelMatrix();

/**
 * @brief set the pixel at the position pos with the color color
 *
 * @param pos the position of the pixel
 * @param color the color of the pixel
 */
            void setPixel(const Vector2u &pos, Pixel &color);

            void setPixel(u_int32_t x, u_int32_t y, Pixel &color);

/**
 * @brief get the pixel at the position pos
 * @param pos the position of the pixel
 * @return Pixel the pixel at the position pos
 */
            inline Pixel &getPixel(Vector2u pos) {
                if (pos.x() > _size.x() || pos.y() > _size.y()) {
                    return _pixelsTab[0];
                }
                return _pixelsTab[pos.y() * _size.x() + pos.x()];
            }

            inline Pixel &getPixel(u_int32_t x, u_int32_t y) {
                if (x > _size.x() || y > _size.y()) {
                    return _pixelsTab[0];
                }
                return _pixelsTab[y * _size.x() + x];
            }

/**
 * @brief Get the To Update object the queue of the pixel to update
 * 
 * @return std::queue<Vector2u>& the queue of the pixel to update
 */
            std::queue<Vector2u> &getToUpdate() { return _toUpdate; }

/**
 * @brief register the pixel at the position pos to be updated
 */

            void registerToUpdate(const Vector2u& pos) {
                _toUpdate.push(pos);
            }

/**
 * @brief register the pixel at the position pos to be updated
 * 
 * @param pos the position of the pixel
 */
            void registerPixelCharToUpdate(Vector2u pos) {
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 2; j++) {
                        registerToUpdate(Vector2u(pos.x() + j, pos.y() + i));
                    }
            }

/**
 * @brief check if the pixel at the position pos need to be updated
 * 
 * @param right the pixel to check
 * @param left the pixel to check
 * @return true if the pixel need to be updated
 * @return false if the pixel don't need to be updated
 */
            static bool isPixelCharToUpdate(Pixel *right, Pixel *left) {
                return (right[0] != left[0] || right[1] != left[1] || right[2] != left[2] || right[3] != left[3] || right[4] != left[4] || right[5] != left[5]);
            }

/**
* @brief get an array of 3*2 pixel at the position pos
* it permited to compute pixel into char
*
* @param pos the position of the pixel
* @return Vector<Pixel> an 3*2 vector of pixel at the position pos
*/
            inline bool getPixelChar(Vector2u pos, Pixel *pixel) {
                return getPixelChar(pos.x(), pos.y(), pixel);
              }

            inline bool getPixelChar(u_int32_t x, u_int32_t y, Pixel *pixel) {
              	u_int32_t sizeX = _size.x();
                if (x + 2 < sizeX && y + 3 < _size.y()) {
                    pixel[0] = _pixelsTab[y * sizeX + x];
                    pixel[1] = _pixelsTab[y * sizeX + x + 1];
                    pixel[2] = _pixelsTab[(y + 1) * sizeX + x];
                    pixel[3] = _pixelsTab[(y + 1) * sizeX + x + 1];
                    pixel[4] = _pixelsTab[(y + 2) * sizeX + x];
                    pixel[5] = _pixelsTab[(y + 2) * sizeX + x + 1];
                    return true;
                } else {
                    return false;
                }
              }

/**
 * @brief Set the Pixel Char object at the position pos
 * 
 * @param pos the position of the pixel
 * @param pixel the pixel to set
 */
            void setPixelChar(Vector2u pos, Pixel *pixel);

            void setPixelChar(u_int32_t x, u_int32_t y, Pixel *pixel);

/**
 * @brief compute the char and the color of the char at the position pos
 * 
 * @param pos the position of the pixel
 * @param pixels the matrix of pixel
 * @return CharColor 
 */
            CharColor computeCharColor(Vector2u pos, Pixel *pixels, CharColor &charColor);

/**
 * @brief get the size of the matrix
 * @return Vector2u the size of the matrix
 */
            [[nodiscard]] inline Vector2u getSize() const { return _size;}

/**
 * @brief Get the Pixels Tab object
 * 
 * @return std::vector<Pixel>& the matrix of pixel
 */
           inline std::vector<Pixel> &getPixelsTab() { return _pixelsTab; }

/**
 * @brief append the matrix of pixel to the matrix
 * 
 * @param pixels the matrix of pixel to append
 */
            void append(std::vector<Pixel> &pixels);

/**
* @brief resize the matrix to the size size
*
* @param size the new size of the matrix
*/
            void resize(Vector2u &size);

/**
 * @brief clear the matrix with black pixel
 *
 */
            void clear();

            inline Pixel* getRawPixelData() {
                return _pixelsTab.data();
            }

/**
 * @brief operator- to crop the matrix
 * 
 * @param crop the crop to apply
 * @return PixelMatrix the new matrix
 */
            PixelMatrix operator-(std::optional<RectU> crop)
            {
                if (!crop.has_value())
                    return *this;
                PixelMatrix newMatrix = PixelMatrix(Vector2u (crop.value().width(), crop.value().height()));
                for (u_int32_t y = 0; y < crop.value().height(); y++) {
                    for (u_int32_t x = 0; x < crop.value().width(); x++) {
                        newMatrix.setPixel(Vector2u(x, y), getPixel(Vector2u(x + crop.value().x(), y + crop.value().y())));
                    }
                }
                return newMatrix;
            }

            bool  operator==(const PixelMatrix &other) const {
                return _size == other._size && _pixelsTab == other._pixelsTab;
            }

            bool  operator!=(const PixelMatrix &other) const {
                return !(*this == other);
            }
        private:
            Vector2u _size; /*!< the size of the matrix */
            std::vector<Pixel> _pixelsTab; /*!< the matrix of pixel */
            std::vector<Pixel> _pixelsTabBlackScreen; /*!< the copy of the matrix of pixel */
            std::queue<Vector2u> _toUpdate; /*!< the queue of the pixel to update */
			std::map<Pixel, std::vector<std::pair<int, int>>> _pixelGroups; // a stocker

    };
}

#endif //TDLPixelMatrix_HPP