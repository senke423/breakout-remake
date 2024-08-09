//
// Created by sybilvane on 5/10/24.
//

#include "Ball.h"

void Ball::render(sf::RenderTarget &renderTarget) {
    renderTarget.draw(sprite);
}

void Ball::initTexture() {
    ball1.loadFromFile("../res/Sprites/ball1.png");
    ball2.loadFromFile("../res/Sprites/ball2.png");
    ball3.loadFromFile("../res/Sprites/ball3.png");
    ball4.loadFromFile("../res/Sprites/ball4.png");
}

void Ball::setCoords(float x, float y) {
    // x and y coords represent TOP LEFT CORNER, not center
    this->x = x;
    this->y = y;
    sprite.setPosition(this->x, this->y);
}

Ball::Ball(unsigned short version, float speed_x, float speed_y) : follow_paddle(true), x(0), y(0) {
    initTexture();

    switch (version) {
        case 0:
            sprite.setTexture(ball1);
            ball_width = 150;
            ball_height = 150;
            break;
        case 1:
            sprite.setTexture(ball2);
            ball_width = 156;
            ball_height = 150;
            break;
        case 2:
            sprite.setTexture(ball3);
            ball_width = 168;
            ball_height = 168;
            break;
        case 3:
            sprite.setTexture(ball4);
            ball_width = 168;
            ball_height = 162;
            break;
    }

    hit_sound.loadFromFile("../res/Sounds/hit3.wav");
    sound.setVolume(master_volume);
    sound.setBuffer(hit_sound);
    this->velocity.x = speed_x;
    this->velocity.y = speed_y;

    float scale_factor = 3;
    ball_width /= scale_factor;
    ball_height /= scale_factor;
    sprite.setScale(1/scale_factor, 1/scale_factor);
}

Ball::~Ball() {

}

// TRUE -> out of screen
// FALSE -> NOT out of screen
bool Ball::update(float delta_time, Paddle* paddle, std::vector<Block*> blocks) {
    if (follow_paddle){
        sf::Vector2f paddle_middle = paddle->getMiddle();
        setCoords(paddle_middle.x - ball_width/2, paddle_middle.y - ball_height);
        return false;
    }

    x += velocity.x * delta_time;
    y += velocity.y * delta_time;

    for (auto* i : blocks){
        sf::Vector2f block_coords = i->getCoords();
        sf::Vector2f block_dims;
        block_dims.x = i->getWidth();
        block_dims.y = i->getHeight();
    }

    if (y > window_height)
        return true;

    if (x <= 0){
        // BOUNCING OFF OF THE LEFT EDGE
        x = 0;
        velocity.x = -velocity.x;
        sound.play();
    }
    if (x >= window_width - ball_width){
        // BOUNCING OFF OF THE RIGHT EDGE
        x = window_width - ball_width;
        velocity.x = -velocity.x;
        sound.play();
    }
    if (y <= 0){
        // BOUNCING OFF OF THE TOP EDGE
        y = 0;
        velocity.y = -velocity.y;
        sound.play();
    }
    sprite.setPosition(x, y);
    return false;
}

void Ball::setFollowPaddle(bool value) {
    follow_paddle = value;
}

void Ball::setVelocity(float x, float y) {
    this->velocity.x = x;
    this->velocity.y = y;
}

float Ball::getWidth() {
    return ball_width;
}

float Ball::getHeight() {
    return ball_height;
}

float Ball::getX() {
    return x;
}

float Ball::getY() {
    return y;
}

sf::Vector2f Ball::getVelocity() {
    return velocity;
}

void Ball::setVelocity(sf::Vector2f velocity) {
    this->velocity = velocity;
}

sf::Vector2f Ball::getMiddle() {
    return {x + getWidth()/2, y + getHeight()/2};
}

bool Ball::getFollowPaddle() {
    return follow_paddle;
}

void Ball::reverseYVelocity() {
    velocity.y *= -1;
}

void Ball::reverseXVelocity() {
    velocity.x *= -1;
}
