
#ifndef TDL_DRAWABLE_HPP
    #define TDL_DRAWABLE_HPP

#include "TDL/Matrix/PixelMatrix.hpp"
#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Rect.hpp"

namespace tdl {

/**
 * @class Drawable
 * @brief class to draw a drawable object
 */
    class Drawable {
        public:
    /**
     * @brief Construct a new Drawable object
     * is the default constructor of the drawable class
     *  @overload
     */
            Drawable() : _matrix(Vector2u(0, 0)), _OldMatrix(Vector2u(0, 0)) { }

    /**
     * @brief Construct a new Drawable object
     * 
     * @param size the size of the drawable
     * @overload
     */
            Drawable(Vector2u &size) : _matrix(size), _OldMatrix(size) { }

    /**
     * @brief Construct a new Drawable object
     * 
     * @param size the size of the drawable
     * @param rect the rect of the drawable
     * @overload
     */
            Drawable(Vector2u &size, RectU &rect) : _matrix(size), _OldMatrix(size), _rect(rect) { }

    /**
     * @brief Construct a new Drawable object
     * 
     * @param matrix the matrix of the drawable object
     */
            Drawable(PixelMatrix &matrix) : _matrix(matrix), _OldMatrix(matrix) { }

    /**
     * @brief Construct a new Drawable object
     * 
     * @param matrix the matrix of the drawable object
     * @param rect the rect of the drawable object
     */
            Drawable(PixelMatrix &matrix, RectU &rect) : _matrix(matrix), _OldMatrix(matrix), _rect(rect) { }

    /**
     * @brief Virtual function to draw the drawable object
     * 
     * @param drawable the drawable object to draw the object on
     */
            virtual void draw(Drawable *drawable) = 0;

    /**
     * @brief Get the Matrix object
     * 
     * @return PixelMatrix& the Matrix object
     */
            PixelMatrix &getMatrix() { return _matrix; }

    /**
     * @brief Get the Old Matrix object
     * 
     * @return PixelMatrix& the Old Matrix object
     */
            PixelMatrix &getOldMatrix() { return _OldMatrix; }

    /**
     * @brief Set the Matrix object
     * 
     * @param matrix the matrix to set
     */
            void setMatrix(PixelMatrix &matrix) { _matrix = matrix; }

    /**
     * @brief Set the Old Matrix object
     * 
     * @param matrix the matrix to set
     */
            void setOldMatrix(PixelMatrix &matrix) { _OldMatrix = matrix; }

    /**
     * @brief Get the Pixel object at the position pos
     * 
     * @param pos the position of the pixel
     * @return Pixel& the pixel at the position pos
     */
            Pixel &getPixel(Vector2u pos) { return _matrix.getPixel(pos); }

    /**
     * @brief Set the Pixel object
     * 
     * @param pos the position of the pixel
     * @param color the color of the pixel
     */
            void setPixel(Vector2u pos, Pixel color) { _matrix.setPixel(pos, color); }

    /**
     * @brief Get the Old Pixel object
     * 
     * @param pos the position of the pixel
     * @return Pixel& 
     */
            Pixel &getOldPixel(Vector2u pos) { return _OldMatrix.getPixel(pos); }

    /**
     * @brief Set the Old Pixel object
     * 
     * @param pos the position of the pixel
     * @param color the color of the pixel
     */
            void setOldPixel(Vector2u pos, Pixel color) { _OldMatrix.setPixel(pos, color); }

    /**
     * @brief Get the Rect object
     * 
     * @return RectU& the rect object
     */
            RectU &getRect() { return _rect; }

    /**
     * @brief Set the Rect object
     * 
     * @param rect the rect to set
     */
            void setRect(RectU rect) { _rect = rect; }

    /**
     * @brief Set the Update object
     * 
     * @param update the update to set
     */
            void setUpdate(bool update) { this->update = update; }

    /**
     * @brief Get the Update object
     * 
     * @return true if the object is to update
     * @return false if the object is not to update
     */
            bool getUpdate() { return update; }

    protected:
    
        PixelMatrix _matrix; /*!< the matrix of the drawable object */
        PixelMatrix _OldMatrix; /*!< the old matrix of the drawable object */
        RectU _rect; /*!< the rect of the drawable object */
        bool update = true; /*!< the update of the drawable object */
    };
}

#endif //TDL_DRAWABLE_HPP
