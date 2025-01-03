
#ifndef TEXTURE_HPP
    #define TEXTURE_HPP

#include <string>
#include <vector>
#include <optional>

#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Graphics/Drawable/Sprite/Texture/Loader/TextureLoader.hpp"
#include "TDL/Utils/Math/Vector.hpp"

#include <TDL/Utils/Matrix/Matrix.hpp>

namespace tdl {

/**
 * @class Texture
 * @brief class to handle texture
 * 
 */
class Texture : public TextureLoader {
        public : 


    /**
     * @brief Destroy the Texture object
     * 
     */
            ~Texture();

    /**
     * @brief get the pixel at the position pos
     * 
     * @param pos the position of the pixel
     * @return Pixel the pixel at the position pos
     */
            Pixel getOriginalPixel(Vector2u &pos);

            Matrix<Pixel> &getTextureData() { return _originalImageData; }

  /**
* @brief Construct a new Texture:: Texture object
*
* @param path the path to the png file to load
* @param scale the scale of the texture
* @param repeat the repeat of the texture
*/
  Texture(std::string path);

  /**
   * @brief function to create a texture from a vector of pixel
   *
   * @param pixelData the vector of pixel to create the texture
   * @param scale the scale of the texture
   * @param repeat the repeat of the texture
   */
  Texture(Pixel *pixelData, Vector2u &size);

    /**
     * @brief Get the Rect object
     * 
     * @return std::optional<RectU>& 
     */

    /**
     * @brief reset the rect of the texture
     * 
     */
        private :

    /**
     * @brief load the image form in pixelData. The load is based from the TextureLoader data
     * @note if you call this function out of the constructor, you can reload the image
     */
            void loadPixels();
    
            Matrix<Pixel> _originalImageData; /*!< the original image data */

    };
}
#endif // TEXTURE_HPP