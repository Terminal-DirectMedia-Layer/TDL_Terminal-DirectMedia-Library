

#ifndef SPRITE_HPP
    #define SPRITE_HPP

#include <optional>

#include "TDL/Window/Window.hpp"
#include "TDL/Drawable/Texture/Texture.hpp"
#include "TDL/Vector.hpp"
#include "TDL/Rect.hpp"
#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Matrix/Transformation.hpp"
#include "TDL/Drawable/Drawable.hpp"

namespace tdl {

    /**
     * @class Sprite
     * @brief Sprite class
     * class for sprite management
     */
    class Sprite : public Transformable, public Drawable {
        public :

        /**
         * @brief Create a Sprite object
         * 
         * @param texture the texture of the sprite
         * @param pos the position of the sprite
         * @return Sprite* a pointer to the sprite
         * @overload
         */
            static Sprite *createSprite(Texture *texture, Vector2u &pos);

            static Sprite *createSprite(tdl::Texture *texture, tdl::Vector2u pos);


        /**
         * @brief Create a Sprite object
         * 
         * @param texture the texture of the sprite
         * @param pos the position of the sprite
         * @param rect the rect of the sprite
         * @return Sprite* a pointer to the sprite
         */
            static Sprite *createSprite(Texture *texture, Vector2u &pos, RectU &rect);

        /**
         * @brief Destroy the Sprite object
         * 
         */
            ~Sprite() override;

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
            void draw(Drawable *drawable) override;

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

        private : 

        /**
         * @brief Construct a new Sprite object
         * 
         * @param texture The texture of the sprite
         * @param pos The position of the sprite
         * @overload
         */
            Sprite(Texture *texture, Vector2u &pos);

        /**
         * @brief Construct a new Sprite object
         * 
         * @param texture The texture of the sprite
         * @param pos The position of the sprite
         * @param rect The rect of the sprite
         */
            Sprite(Texture *texture, Vector2u &pos, RectU &rect);

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
    };
}

#endif // SPRITE_HPP