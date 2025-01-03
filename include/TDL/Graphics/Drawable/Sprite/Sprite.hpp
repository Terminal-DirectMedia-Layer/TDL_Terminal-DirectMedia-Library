

#ifndef SPRITE_HPP
    #define SPRITE_HPP

#include <optional>

#include "TDL/Graphics/Drawable/Sprite/Texture/Texture.hpp"
#include "TDL/Utils/Math/Vector.hpp"
#include "TDL/Utils/Math/Rect.hpp"
#include "TDL/Graphics/Drawable/Pixel/Pixel.hpp"
#include "TDL/Graphics/FrameBuffer/Feature/Transformation.hpp"
#include "TDL/Graphics/Drawable/ADrawable.hpp"

namespace tdl {

    /**
     * @class Sprite
     * @brief Sprite class
     * class for sprite management
     */
    class Sprite : public Transformable, public ADrawable {
        public :

      /**
       * @brief Construct a new Sprite object
       *
       * @param texture The texture of the sprite
       * @param pos The position of the sprite
       * @overload
       */
          Sprite(Texture *texture, Vector2u pos);

      /**
       * @brief Construct a new Sprite object
       *
       * @param texture The texture of the sprite
       * @param pos The position of the sprite
       * @param rect The rect of the sprite
       */
          Sprite(Texture *texture, Vector2u pos, RectU rect);

        /**
         * @brief Destroy the Sprite object
         * 
         */
            ~Sprite() override;

          void setRext(RectU &rect) {
            _rect = rect;
            _matrix = _texture->getTextureData();
            Vector2u size = _texture->getSize();
            _matrix.resize(size);
            _matrix = _matrix - rect;
          }

        /**
         * @brief Set the Texture object
         * 
         * @param texture the texture to set
         */
            void setTexture(Texture *texture) { _texture = texture;}

        /**
         * @brief Set the Tint object
         * 
         * @param tint the tint to set
         * @deprecated
         */
            void setTint(Pixel tint) { _tint = tint;}

        /**
         * @brief Get the Texture object
         * 
         * @return Texture* the texture
         */
            Texture *getTexture() { return _texture;}

        /**
         * @brief Get the Tint object
         * 
         * @return Pixel the tint
         * @deprecated
         */
            Pixel getTint() { return _tint.value_or(Pixel(0, 0, 0, 0));}

        /**
         * @brief draw the sprite
         * 
         * @param drawable the drawable to draw on
         */

            /**
             * @brief lerp between two pixel
             * 
             * @param a the first pixel
             * @param b the second pixel
             * @param t teh delta
             * @return Pixel 
             */
            static Pixel lerp(Pixel a, Pixel b, double t);

        /**
         * @brief check if the sprite is intersecting with a point
         * 
         * @param point the point to check
         */
            bool isIntersect(const Vector2i &point);

            void draw(Window *d) override;

        private : 

        /**
         * @brief check if the pixel is black
         * 
         * @param pixel the pixel to check
         * @return true if the pixel is black
         * @return false if the pixel is not black
         */
            static bool isBlackPixel(Pixel pixel);

            Texture *_texture; /* !< the texture of the sprite */
            Vector2u _pos; /* !< the position of the sprite */
            std::optional<Pixel> _tint; /* !< the tint of the sprite */
            RectU _rect; /* !< the rect of the sprite */
    };
}

#endif // SPRITE_HPP