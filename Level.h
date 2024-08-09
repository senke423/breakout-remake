//
// Created by sybilvane on 5/8/24.
//

#ifndef INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_LEVEL_H
#define INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_LEVEL_H

#include "Headers.h"
#include "Paddle.h"
#include "Block.h"
#include "Ball.h"
#include <random>

class Level {
    sf::Clock stopwatch;
    sf::Clock countdown_clock;
    Paddle* paddle;
    Ball* ball;
    std::vector<sf::Drawable*> pause_text;
    std::vector<sf::Text> hud;
    std::vector<sf::Texture> gif_frames;
    std::vector<Block*> blocks;
    sf::Sound sound;
    sf::SoundBuffer select;
    sf::SoundBuffer confirm;
    sf::SoundBuffer game_over_sound;
    sf::SoundBuffer life_lost;
    sf::SoundBuffer beep2;
    sf::SoundBuffer beep3;
    sf::SoundBuffer shooting_sound;
    sf::SoundBuffer pause_on_sound;
    sf::SoundBuffer pause_off_sound;
    sf::SoundBuffer paddle_hit;
    sf::Font font;
    sf::Text message;
    sf::Sprite gif_sprite;
    sf::Clock frame_clock;
    unsigned int elapsed_time;
    unsigned int score;
    unsigned int blocks_destroyed;
    unsigned int message_size;
    unsigned short lives;
    unsigned short lives_lost;
    unsigned short back_option;
    unsigned short current_frame;
    float elapsed_countdown;
    float delta_time;
    float acceleration_coeff;
    float frame_duration = .12f;
    bool paused;
    bool countdown;
    bool countdown_flags[4];
    bool ball_dropped;
    bool game_over;
    const sf::Vector2f start_velocity = {0, -10};

    void initSounds();
    void initFont();
    void initPauseScreen();
    void initHud();
    void initLevel();
    void initBackground();

    void recenter(std::wstring new_text);

public:
    Level();
    ~Level();

    unsigned int getElapsedTime();
    unsigned int getBlocksDestroyed();
    int getLivesLost();

    void movePaddleLeft();
    void movePaddleRight();
    void shootBall();
    void setDeltaTime(float delta_time);
    bool isPaused() const;
    void backOptionLeft();
    void backOptionRight();
    bool backOptionConfirm();
    void setFps(unsigned int fps);

    void pause();
    void update();
    void render(sf::RenderTarget& renderTarget);

};


#endif //INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_LEVEL_H
