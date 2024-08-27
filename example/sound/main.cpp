/*
** EPITECH PROJECT, 2024
** libNcurse
** File description:
** main.cpp
*/

#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <TDL/Window/Window.hpp>
#include "TDL/Event/Event.hpp"
#include "TDL/Input/InputKeyboard.hpp"
#include "TDL/Sprite/Sprite.hpp"
#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Vector.hpp"
#include "TDL/Text/Font/Font.hpp"
#include "TDL/Text/Text.hpp"
#include <tuple>
#include <queue>
#include "TDL/Matrix/Transform.hpp"
#include "TDL/Sound/Sound.hpp"
#include <chrono>
#include <fstream>

int main()
{
    bool play = true;
    bool run = true;
    auto start = std::chrono::high_resolution_clock::now();
    tdl::Window *win = tdl::Window::createWindow("bird");
    tdl::Texture *tex = tdl::Texture::createTexture("../example/assets/rick.png");
    tdl::Vector2u pos(10, 10);
    tdl::Sprite *sprite = tdl::Sprite::createSprite(tex, tdl::Vector2u(0, 0));
    double rotation = 45.0;
    //tex->rotate(rotation);
    tdl::Sound sound;
    sound.loadFile("../example/assets/rick.mp3");
    sound.play();

    while (run)
    {
        win->clearPixel();
        sprite->draw(win);
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event);) {
            std::cerr << "Event type: " << event.type << std::endl;
            if (event.type == tdl::Event::EventType::KEYPRESSED) {
                std::cerr << "Key code: " << event.key.code << std::endl;
                if (event.key.code == tdl::KeyCodes::KEY_A)
                    run = false;
                if (event.key.code == tdl::KeyCodes::KEY_SPACE){
                    if (play) {
                        sound.pause();
                        play = false;
                    } else {
                        sound.resume();
                        play = true;
                    }
                }
                if (event.key.code == tdl::KeyCodes::KEY_UP) {
                    rotation += 0.1;
                    tex->setRotation(rotation);
                }
                if (event.key.code == tdl::KeyCodes::KEY_DOWN) {
                    rotation -= 0.1;
                    tex->setRotation(rotation);
                }
            }    
        }
        win->printFrameRate();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::ofstream time_file("execution_time.txt", std::ios::app);
    time_file << elapsed.count() << std::endl;
    time_file.close();

    return 0;
}
