#ifndef IDRAWMETHODE_HPP
    #define IDRAWMETHODE_HPP

#include "TDL/Graphics/FrameBuffer/FrameBuffer.hpp"

namespace tdl
{
    /**
     * @class IDrawMethode
     * @brief An interface for the draw methode.
     */
    class IDrawMethode
    {
      public:
        /**
         * @brief Destructor for the IDrawMethode class.
         */
        virtual ~IDrawMethode() {}

        /**
         * @brief draws the object.
         */
        virtual void draw(FrameBuffer &buffer) = 0;
    };
}

#endif //IDRAWMETHODE_HPP
