    #include <iostream>
#include <unistd.h>
#include <TDL/Window/TtyDisplay.hpp>
#include <TDL/Drawable/Texture/Texture.hpp>
#include <TDL/Sprite/Sprite.hpp>

int main()
{
    tdl::TtyDisplay *win = tdl::TtyDisplay::createTtyDisplay("WindowName");
    tdl::Texture *tex = tdl::Texture::createTexture("../example/assets/bird.png");
    tdl::Texture *tex2 = tdl::Texture::createTexture("../example/assets/mouse_cursor.png");
    tdl::Sprite *sprite = tdl::Sprite::createSprite(tex, tdl::Vector2u(0, 0));
    tdl::Sprite *sprite2 = tdl::Sprite::createSprite(tex2, tdl::Vector2u(0, 0));
    tdl::RectU rect(0, 0, 32, 32);
	//sprite->setRect(rect);
    int i = 0;
	sprite->setPosition(500, 500);
    sprite2->setScale(0.1, 0.1);
    for (int j = 0; j < 10000; j++)
    {
        win->clearPixel();
        sprite->draw(win);
        sprite2->draw(win);
        //std::cerr << sprite2->getPosition().x() << "," << sprite2->getPosition().y() << std::endl;
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event);) {
            if (event.type == TDL_KEYPRESSED) {
                if (event.key == TDL_KEY_ESC) {
                    delete win;
                    delete sprite;
					delete sprite2;
                    return 0;
                }
            }
            if (event.type == TDL_KEYREPEAT)
            {
                if (event.key == TDL_KEY_UP)
                {
                    sprite->move(tdl::Vector2f(0, -10));
                }
                if (event.key == TDL_KEY_DOWN)
                {
                    sprite->move(tdl::Vector2f(0, 10));
                }
                if (event.key == TDL_KEY_LEFT)
                {
                    sprite->move(tdl::Vector2f(-10, 0));
                }
                if (event.key == TDL_KEY_RIGHT)
                {
                    sprite->move(tdl::Vector2f(10, 0));
                }
            }
            if (event.type == TDL_MOUSEMOVED)
            {
            	sprite2->setPosition(std::max(0, event.mouseMove.x), std::max(0, event.mouseMove.y));

            }

/*
			if (event.type == TDL_WHEELUP) {
					sprite2->rotate(10);
			}
            if (event.type == TDL_WHEELDOWN) {
                sprite2->rotate(-10);
            }
*/
        }
/*
        if (rect.x() >= 32) {
            rect.x() = 0;
            if (rect.y() >= 96) {
                rect.y() = 0;
            } else {
                rect.y() += 32;
            }
        } else {
            rect.x() += 32;
        }
        tex->setRect(rect);
*/
       	//win->printFrameRate();
    }
    return 0;
}