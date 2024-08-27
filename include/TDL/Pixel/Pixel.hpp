

#pragma once

#include <cstdint>
#include <iostream>
#include <algorithm>

#define GET_R(color) ((color >> 24) & 0xFF) /* get the red color value */
#define GET_G(color) ((color >> 16) & 0xFF) /* get the green color value */
#define GET_B(color) ((color >> 8) & 0xFF)  /* get the blue color value */
#define GET_A(color) (color & 0xFF)         /* get the alpha color value */

namespace tdl {
    /**
     * @file Pixel.hpp
     * @brief Pixel structure for window. The objective here is to describe what is a
     * pixel and how it is represented in the window. it handle its color and its operator between color
     */

    /**
     * @struct Pixel
     * @brief the Pixel structure
     */
    struct Pixel {
        uint32_t color;

        /**
         * @brief Construct a new Pixel object
         * The union provide tree way to construct a pixel
         * 1. with the color value
         * 2. with the r, g, b, a value
         * 3. with the default constructor that set the color to 0
         */
        Pixel() : color(0) {}

        /**
         * @brief Construct a new Pixel object
         * 
         * @param r red color value
         * @param g green color value
         * @param b blue color value
         * @param a alpha color value
         */
        Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
            color = (r << 24) | (g << 16) | (b << 8) | a;
        }

        /**
         * @brief Construct a new Pixel object
         * 
         * @param color the color value
         */
        Pixel(uint32_t color) : color(color) {}

        /**
         * @brief operator== overload for pixel
         * 
         * @param other the other pixel to compare
         * @return true if the pixel are equals
         * @return false if the pixel are not equals
         */
        bool operator==(const Pixel &other) const;

        /**
         * @brief operator!= overload for pixel
         * 
         * @param other the other pixel to compare
         * @return true if the pixel are not equals
         * @return false if the pixel are equals
         */
        bool operator!=(const Pixel &other) const;

        /**
         * @brief operator+ overload for pixel
         * this operator is used to blend the color of the pixel with another pixel
         * the other pixel is the pixel to blend with the current pixel
         * @param other the other pixel to blend
         * @return Pixel the new pixel with the blend color
         */
        Pixel operator+(const Pixel &other) const;

        /**
         * @brief operator- overload for pixel
         * this operator is used to blend the color of the pixel with another pixel
         * the other pixel is the pixel to substract blend with the current pixel
         * @param other 
         * @return Pixel the new pixel with the blend color
         */
        Pixel operator-(const Pixel &other) const;

        /**
         * @brief operator* overload for pixel
         * this operator is used to blend the color of the pixel with another pixel
         * the other pixel is the pixel to multiply blend with the current pixel
         * @param other 
         * @return Pixel the new pixel with the blend color
         */
        bool operator<(const Pixel &other) const;
    };

}