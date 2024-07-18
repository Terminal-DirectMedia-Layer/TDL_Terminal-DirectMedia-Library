/*
** EPITECH PROJECT, 2024
** libNcurse
** File description:
** main.cpp
*/

#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <tdl/Window/window.hpp>
#include "tdl/Event/Event.hpp"
#include "tdl/Input/inputKeyboard.hpp"
#include "tdl/Sprite/Sprite.hpp"
#include "tdl/Pixel/Pixel.hpp"
#include "tdl/Vector.hpp"
#include "tdl/Text/Font/Font.hpp"
#include "tdl/Text/Text.hpp"
#include <tuple>
#include <queue>
#include "tdl/Matrix/Transform.hpp"
#include <chrono>
#include <fstream>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    tdl::Window *win = tdl::Window::CreateWindow("bird");
    tdl::Texture *tex = tdl::Texture::createTexture("../example/assets/bird.png");
    tdl::Vector2u pos(10, 10);
    tdl::Sprite *sprite = tdl::Sprite::createSprite(tex, tdl::Vector2u(0, 0));
    //tex->setRect(tdl::RectU(100, 100, 100, 100));
    while (true)
    {
        win->clearPixel();
        sprite->draw(win);
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KeyPressed) {
                if (event.key.code == tdl::KeyCodes::KEY_ESC)
                    delete win;
                    delete sprite;
                    return 0;
                
            }
        }
        //win->printFrameRate();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::ofstream time_file("execution_time.txt", std::ios::app);
    time_file << elapsed.count() << std::endl;
    time_file.close();

    delete win;
    delete sprite;
    delete tex;

    return 0;
}
