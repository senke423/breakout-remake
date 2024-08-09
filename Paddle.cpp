//
// Created by sybilvane on 5/8/24.
//

#include "Paddle.h"

Paddle::Paddle(float x, float y) : x(x), y(y), velocity(35) {
    initTexture();
}

void Paddle::render(sf::RenderTarget &renderTarget) {
    renderTarget.draw(sprite);
}

void Paddle::initTexture() {
    texture.loadFromFile("../res/Sprites/paddle.png");
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Paddle::update(MOVEMENT movement, float delta_time) {
    if (movement == MOVEMENT::LEFT){
        float new_x = x - velocity * delta_time;
        if (new_x < 0)
            x = 0;
        else
            x = new_x;
    }
    else if (movement == MOVEMENT::RIGHT){
        float new_x = x + velocity * delta_time;
        if (new_x + paddle_width > window_width)
            x = window_width - paddle_width;
        else
            x = new_x;
    }
    sprite.setPosition(x, y);
}

float Paddle::getX() {
    return x;
}

float Paddle::getY() {
    return y;
}

// Returns the middle. Same Y coord as paddle's top left corner.
sf::Vector2f Paddle::getMiddle() {
    return {x + paddle_width / 2, y};
}

float Paddle::getWidth() {
    return paddle_width;
}

float Paddle::getHeight() {
    return paddle_height;
}


