//
// Created by sybilvane on 5/8/24.
//

#ifndef INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_PADDLE_H
#define INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_PADDLE_H

#include "Headers.h"

class Paddle {
    sf::Sprite sprite;
    sf::Texture texture;
    float x;
    float y;
    float paddle_width = 156;
    float paddle_height = 36;
    float velocity;

    void initTexture();
public:
    Paddle(float x, float y);

    float getX();
    float getY();
    float getWidth();
    float getHeight();
    sf::Vector2f getMiddle();

    void update(MOVEMENT movement, float delta_time);
    void render(sf::RenderTarget& renderTarget);
};


#endif //INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_PADDLE_H
