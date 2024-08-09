//
// Created by sybilvane on 5/10/24.
//

#ifndef INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_BALL_H
#define INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_BALL_H

#include "Headers.h"
#include "Paddle.h"
#include "Block.h"


class Ball {
    float x;
    float y;
    float ball_width;
    float ball_height;
    bool follow_paddle;
    sf::Texture ball1;
    sf::Texture ball2;
    sf::Texture ball3;
    sf::Texture ball4;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    sf::SoundBuffer hit_sound;
    sf::Sound sound;

    void initTexture();

public:
    Ball(unsigned short version, float speed_x, float speed_y);
    ~Ball();

    void setCoords(float x, float y);
    void setFollowPaddle(bool value);
    void setVelocity(float x, float y);
    void setVelocity(sf::Vector2f velocity);

    float getX();
    float getY();
    float getWidth();
    float getHeight();
    sf::Vector2f getVelocity();
    sf::Vector2f getMiddle();
    bool getFollowPaddle();
    void reverseYVelocity();
    void reverseXVelocity();

    bool update(float delta_time, Paddle* paddle, std::vector<Block*> blocks);
    void render(sf::RenderTarget& renderTarget);
};


#endif //INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_BALL_H
