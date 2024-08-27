
#ifndef TEXTURELOADER_HPP
    #define TEXTURELOADER_HPP

#include <string>
#include <png.h>

#include "TDL/Vector.hpp"

namespace tdl {
    /**
     * @class TextureLoader
     * @brief class to load a texture
     * it handle the loading of a texture
     */
    class TextureLoader {
        public : 
            Vector2u getSize() { return _size; }

        protected :
    /**
     * @brief Construct a new tdl::Texture Loader::Texture Loader object
     * 
     * @param path the path to the png file to load 
     * @note on this version, the texture loader only loads png files
     */
            TextureLoader(std::string path);

    /**
     * @brief Destroy the tdl::Texture Loader::Texture Loader object
     * 
     */
            ~TextureLoader();

    /**
     * @brief load the texture from the given path
     * 
     * @param path the path to the png file to load
     * @note this function is the init of the texture loader
     */
            void loadTexture();

    /**
     * @brief load the texture from the stock path
     * @overload
     */
            void loadTexture(std::string path);

    /**
     * @brief check if the file is a png file
     * 
     * @param fd the file to check
     * @return true if the file is a png file
     * @return false if the file is not a png file
     */
            bool isPng(std::string path);
            
            //png variables
            std::string _path; /*!< the path to the png file */
            png_infop _info_ptr; /*!< the info pointer of the png file */
            png_bytepp _row_pointers; /*!< the row pointers of the png file */
            png_structp _png_ptr; /*!< the png pointer of the png file */
            Vector2u _size; /*!< the size of the png file */
            int _channels; /*!< the channels of the png file */
            int _bit_depth; /*!< the bit depth of the png file */
    };
}

#endif /* !TEXTURELOADER_HPP */
