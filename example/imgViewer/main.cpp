/*
** EPITECH PROJECT, 2024
** libNcurse
** File description:
** main.cpp
*/

#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <tdl/Utils/SubShell.hpp>
#include <tdl/Window/terminalDisplay.hpp>
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
#include <regex>

void testcustomCommand(tdl::TerminalDisplay *win, int argc, char **argv)
{
    for (int i = 0; i < argc; i++) {
        std::cerr << argv[i] << std::endl;
    }
}


int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    tdl::TerminalDisplay *win = tdl::TerminalDisplay::CreateTerminalDisplay("test", "/dev/tty", 20);
    tdl::Texture *tex = tdl::Texture::createTexture("../example/assets/Spinner.png");
    tdl::Vector2u pos(10, 10);
    tdl::Sprite *sprite = tdl::Sprite::createSprite(tex, tdl::Vector2u(0, 0));
    win->registerCommand("test", testcustomCommand);
    std::vector<std::tuple<tdl::Sprite *, bool>> sprites;
    tdl::RectU rect(0, 0, 32, 32);
    tdl::Vector2u mouse(0, 0);
    std::regex pngRegex("^(.*\\/)?(.+\\.png )$");
    bool isIntersect = false;
    sprite->setRect(rect);
    std::string s = "";
    while (true)
    {
        win->clearPixel();
        for (auto &s : sprites)
            std::get<0>(s)->draw(win);
        sprite->draw(win);
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event, &pngRegex);) {
            if (event.type == tdl::Event::EventType::KeyPressed) {
                if (event.key.code == tdl::KeyCodes::KEY_ESC)
                    return 0;
                if (event.key.code == tdl::KeyCodes::KEY_RIGHT) {
                    pos += tdl::Vector2u(1, 0);
                }
                if (event.key.code == tdl::KeyCodes::KEY_LEFT) {
                    pos -= tdl::Vector2u(1, 0);
                }
                if (event.key.code == tdl::KeyCodes::KEY_UP) {
                    pos -= tdl::Vector2u(0, 1);
                }
                if (event.key.code == tdl::KeyCodes::KEY_DOWN) {
                    pos += tdl::Vector2u(0, 1);
                }
                if (event.key.code == tdl::KeyCodes::KEY_ENTER) {
                    s.append("\n");
                    s = "";
                }
                    s.append(1, event.key.code);
            }
            if (event.type == tdl::Event::EventType::MouseButtonPressed && event.mouseButton.button == tdl::MouseButton::LEFT) {

                for (auto &s : sprites) {
                    if (std::get<0>(s)->isIntersect(tdl::Vector2i(event.mouseButton.x, event.mouseButton.y))) {
                        std::get<1>(s) = true;
                    } 
                }
                if (sprite->isIntersect(tdl::Vector2i(event.mouseButton.x, event.mouseButton.y))) {
                    isIntersect = true;
                } else {
                    win->setHeight(win->getHeight() + 3);
                }
            }
            if (event.type == tdl::Event::EventType::MouseButtonReleased) {
                for (auto &s : sprites) {
                    std::get<1>(s) = false;
                }
                isIntersect = false;
            }
            if (event.type == tdl::Event::EventType::MouseMoved) {
                mouse = tdl::Vector2u(event.mouseMove.x, event.mouseMove.y);
            }
        }
        for (auto &s : sprites) {
            if (std::get<1>(s)) {
                std::get<0>(s)->setPosition(mouse);
            }
        }
        if (isIntersect) {
            pos = mouse;
        }
        sprite->setPosition(pos);
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
        //win->printFrameRate();
        tex->setRect(rect);
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
