//
// Created by sybilvane on 5/7/24.
//

#ifndef INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_GAME_H
#define INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_GAME_H

#include "Headers.h"
#include "Paddle.h"
#include "MainMenu.h"
#include "Level.h"

class Game {
private:
    sf::RenderWindow window;
    sf::Event event;
    sf::Clock frame_clock;
    sf::Clock fps_clock;
    sf::SoundBuffer confirm;
    sf::Sound sound;
    MainMenu* mainMenu;
    Level* level;
    bool is_main_menu;
    bool first_frame_rendered;
    bool going_left;
    bool going_right;
    unsigned int fps_counter;

    void initWindow();
    void initMainMenu();
    void pollEvents();
    void updateConfigFile();

public:
    Game();
    ~Game();

    void update();
    void render();

    const sf::RenderWindow& getWindow() const;
};


#endif //INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_GAME_H
