/*
** EPITECH PROJECT, 2024
** libNcurse
** File description:
** main.cpp
*/

#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <TDL/Window/window.hpp>
#include "TDL/Event/Event.hpp"
#include "TDL/Input/inputKeyboard.hpp"
#include "TDL/Sprite/Sprite.hpp"
#include "TDL/Pixel/Pixel.hpp"
#include "TDL/Vector.hpp"
#include "TDL/Text/Font/Font.hpp"
#include "TDL/Text/Text.hpp"
#include <tuple>
#include <queue>
#include "TDL/Matrix/Transform.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>

int main()
{
    tdl::Window *win = tdl::Window::CreateWindow("pong");
    tdl::Texture *board = tdl::Texture::createTexture("../example/assets/arts/Board.png");
    tdl::Sprite *sprite = tdl::Sprite::createSprite(board, tdl::Vector2u(0, 0));

    tdl::Texture *player = tdl::Texture::createTexture("../example/assets/arts/Player.png");
    tdl::Sprite *playerSprite = tdl::Sprite::createSprite(player, tdl::Vector2u(0, 0));

    tdl::Texture *ball = tdl::Texture::createTexture("../example/assets/arts/Ball.png");
    tdl::Sprite *ballSprite = tdl::Sprite::createSprite(ball, tdl::Vector2u(win->getWidth(), win->getHeight()));
    tdl::Vector2u ballPos = tdl::Vector2u(win->getWidth() / 2, win->getHeight() / 2);
    ballSprite->setPosition(ballPos);
    ballSprite->setOrigin(tdl::Vector2f(ball->getSize().x(), ball->getSize().y()));
    float ballSpeed = 5;
    float angle = 45;
    tdl::Vector2i direction = tdl::Vector2i(-1, 1);

    tdl::Vector2f scale;
    tdl::Vector2u playerPos = tdl::Vector2u(0, win->getHeight() / 2);
    playerSprite->setPosition(playerPos);

    while (true)
    {
        scale.x() = static_cast<float>(win->getWidth()) / board->getSize().x();
        scale.y() = static_cast<float>(win->getHeight()) / board->getSize().y();
        board->setScale(scale);
        player->setScale(tdl::Vector2f(static_cast<float>(win->getWidth()) / (50 * player->getSize().x()), (static_cast<float>(win->getHeight()) / (5 * player->getSize().y()))));

        ball->setScale(tdl::Vector2f(0.5, 0.5));
        win->clearPixel();
        sprite->draw(win);
        ballSprite->draw(win);
        playerSprite->draw(win);
        win->update();
        win->draw();
        for(tdl::Event event; win->pollEvent(event);) {
            if (event.type == tdl::Event::EventType::KeyPressed) {
                if (event.key.code == tdl::KeyCodes::KEY_ESC)
                    return 0;
                if (event.key.code == tdl::KeyCodes::KEY_UP) {
                    playerPos.y() = playerPos.y() - 15;
                    playerSprite->move(tdl::Vector2f(0, -15));
                }
                if (event.key.code == tdl::KeyCodes::KEY_DOWN) {
                    playerPos.y() = playerPos.y() + 15;
                    playerSprite->move(tdl::Vector2f(0, 15));
                }
            }
        }
        float angleRad = angle * M_PI / 180.0f;
        float velocityX = ballSpeed * cos(angleRad) * direction.x();
        float velocityY = ballSpeed * sin(angleRad) * direction.y();
        ballPos.x() += velocityX;
        ballPos.y() += velocityY;
        ballSprite->setPosition(ballPos);
        std::cerr << "balpos: " << ballPos.x() << " " << ballPos.y() << std::endl;
        std::cerr << "winside: " << win->getWidth() << " " << win->getHeight() << std::endl;
        if (ballPos.y() < 0 ||ballPos.y() > win->getHeight() * 2)
            direction.y() *= -1;
        if (ballPos.x() > win->getWidth() * 2)
            direction.x() *= -1;
        std::cerr << "bounds: " << ballPos.x() << " " << ballPos.y() << std::endl;
        std::cerr << "playerbounds: " << playerPos.x() << " " << playerPos.y() << std::endl;
        std::cerr << "playersize: " << player->getSize().x() << " " << player->getSize().y() << std::endl;
        std::cerr << "playerscale: " << player->getScale().x() << " " << player->getScale().y() << std::endl;
        if (ballPos.x() + ballSprite->getOrigin().x() <= playerPos.x() + player->getSize().x() * 2 && ballPos.y() >= playerPos.y() && ballPos.y() <= playerPos.y() + player->getSize().y()) {
            direction.x() *= -1;
        }
        if (ballPos.x() <= ballSprite->getOrigin().x()){
            ballPos = tdl::Vector2u(win->getWidth(), win->getHeight());
        }
        //check collision

    }
    return 0;
}
