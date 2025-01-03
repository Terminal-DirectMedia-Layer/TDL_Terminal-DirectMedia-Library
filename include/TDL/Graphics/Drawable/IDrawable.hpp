
#ifndef IDRAWABLE_HPP
    #define IDRAWABLE_HPP

namespace tdl {

        class Window;

        class IDrawable {
            public:
                virtual void draw(Window *d) = 0;
        };
};

#endif //IDRAWABLE_HPP
