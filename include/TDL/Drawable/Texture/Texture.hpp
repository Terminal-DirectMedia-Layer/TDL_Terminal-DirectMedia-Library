
#ifndef TEXTURE_HPP
    #define TEXTURE_HPP

#include <string>
#include <vector>
#include <optional>

#include "TextureLoader.hpp"
#include "TDL/Vector.hpp"
#include "TDL/Rect.hpp"
#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Matrix/Transformation.hpp"
#include "TDL/Drawable/Drawable.hpp"

namespace tdl {

/**
 * @class Texture
 * @brief class to handle texture
 * 
 */
class Texture : public TextureLoader, public Transformable {
        public : 


    /**
     * @brief Destroy the Texture object
     * 
     */
            ~Texture() override;

    /**
     * @brief function to create a texture
     * 
     * @param path the path to the png file to load
     * @return Texture* the texture created
     * @overload
     */
            static Texture *createTexture(std::string path);

    /**
     * @brief function to create a texture
     * 
     * @param path the path to the png file to load
     * @param repeat the repeat of the texture
     * @return Texture* the texture created
     * @overload
     */
            static Texture *createTexture(std::string &path, bool repeat);

    /**
     * @brief function to create a texture from a vector of pixel
     *
     * @param pixelData the vector of pixel to create the texture
     * @return Texture* the texture created
     * @overload
     */
            static Texture *createTextureFromVector(Pixel *pixelData, Vector2u &size);

    /**
     * @brief function to create a texture from a vector of pixel
     *
     * @param pixelData the vector of pixel to create the texture
     * @param scale the scale of the texture
     * @return Texture* the texture created
     * @overload
     */
            static Texture *createTextureFromVector(Pixel *pixelData, Vector2u &size, Vector2f &scale);

    /**
     * @brief function to create a texture from a vector of pixel
     *
     * @param pixelData the vector of pixel to create the texture
     * @param repeat the repeat of the texture
     * @return Texture* the texture created
     */
            static Texture *createTextureFromVector(Pixel *pixelData, Vector2u &size, bool repeat);

    /**
     * @brief function to create a texture from a vector of pixel
     *
     * @param pixelData the vector of pixel to create the texture
     * @param scale the scale of the texture
     * @param repeat the repeat of the texture
     * @return Texture* the texture created
     */
            static Texture *createTextureFromVector(Pixel *pixelData, Vector2u &size, Vector2f &scale, bool repeat);

    /**
     * @brief get the pixel at the position pos
     * 
     * @param pos the position of the pixel
     * @return Pixel the pixel at the position pos
     */
            Pixel getOriginalPixel(Vector2u &pos);

    /**
     * @brief Get the Original Image Data object
     * 
     * @return PixelMatrix& an reference to the original image data
     */
            PixelMatrix &getOriginalImageData() { return _originalImageData; }

    /**
     * @brief Set the Rect the Texture
     * 
     * @param rect the rect to set
     * @overload
     */
             void setRect(const RectU& rect) { _rect = rect; }

    /**
     * @brief Set the Rect object
     * 
     * @param x the x upper right position of the rect
     * @param y the y upper right position of the rect
     * @param width the width of the rect
     * @param height the height of the rect
     */
             void setRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { _rect = RectU(x, y, width, height); }

    /**
     * @brief Get the Rect object
     * 
     * @return std::optional<RectU>& 
     */
             std::optional<RectU> &getRect() { return _rect; }

    /**
     * @brief reset the rect of the texture
     * 
     */
             void resetRect() { _rect = std::nullopt; }
        private :

    /**
     * @brief Construct a new Texture:: Texture object
     * 
     * @param path the path to the png file to load
     * @param scale the scale of the texture
     * @param repeat the repeat of the texture
     */
            Texture(std::string &path, bool repeat);

    /**
     * @brief function to create a texture from a vector of pixel
     *
     * @param pixelData the vector of pixel to create the texture
     * @param scale the scale of the texture
     * @param repeat the repeat of the texture
     */
            Texture(Pixel *pixelData, Vector2u &size, Vector2f &scale, bool repeat);

    /**
     * @brief load the image form in pixelData. The load is based from the TextureLoader data
     * @note if you call this function out of the constructor, you can reload the image
     */
            void loadPixels();
    
            PixelMatrix _originalImageData; /*!< the original image data */
            std::optional<RectU> _rect; /*!< the rect of the texture */

    };
}
#endif // TEXTURE_HPP