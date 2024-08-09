//
// Created by sybilvane on 5/7/24.
//

#include "Game.h"

void Game::initWindow() {
    window.create(sf::VideoMode(window_width, window_height),
                        "Breakout!", sf::Style::Close);
    window.setFramerateLimit(90);
}

const sf::RenderWindow &Game::getWindow() const {
    return window;
}

Game::Game() : is_main_menu(true), going_left(false), going_right(false), fps_counter(0),
mainMenu(nullptr), level(nullptr) {
    initWindow();
    initMainMenu();

    confirm.loadFromFile("../res/Sounds/confirm1.wav");
    sound.setVolume(master_volume);
}

void Game::update() {
    pollEvents();

    if (!mainMenu){
        if (going_left)
            level->movePaddleLeft();
        else if (going_right)
            level->movePaddleRight();
        level->update();
    }

}

void Game::render() {
    window.clear();

    // render the game here
    if (is_main_menu){
        mainMenu->render(window);
    } else {
        // GAMEPLAY
        fps_counter++;
        if (fps_clock.getElapsedTime().asSeconds() >= 1){
            fps_clock.restart();
            level->setFps(fps_counter);
            fps_counter = 0;
        }

        if (!first_frame_rendered){
            frame_clock.restart();
            fps_clock.restart();
            first_frame_rendered = true;
        } else {
            level->setDeltaTime(frame_clock.getElapsedTime().asSeconds() + 1);
            frame_clock.restart();
        }
        level->render(window);
    }

    window.display();
}

Game::~Game() {

}

void Game::pollEvents() {
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();

        // MAIN MENU
        if (is_main_menu && event.type == sf::Event::KeyPressed){
            switch (event.key.code){
                case sf::Keyboard::Up:
                    mainMenu->goUp();
                    break;
                case sf::Keyboard::Down:
                    mainMenu->goDown();
                    break;
                case sf::Keyboard::Escape:
                    mainMenu->goBack();
                    break;
                case sf::Keyboard::Enter:
                    int ret = mainMenu->doAction();

                    if (ret == 1){
                        // Continue game
                    }
                    else if (ret == 2){
                        // New game started
                        level = new Level();
                        first_frame_rendered = false;
                        is_main_menu = false;
                        delete mainMenu;
                        mainMenu = nullptr;
                    }
                    else if (ret == 3 || ret == 4){
                        sound.setBuffer(confirm);
                        sound.play();
                    }
                    else if (ret == 5){
                        // 'Zatvori' (Close) is clicked.
                        sound.setBuffer(confirm);
                        sound.play();
                        sf::sleep(sf::milliseconds(200));
                        window.close();
                    }
                    break;
            }
        }

        // GAMEPLAY
        if (!is_main_menu){
            if (event.type == sf::Event::KeyPressed){
                switch (event.key.code){
                    case sf::Keyboard::Escape:
                        level->pause();
                        break;
                    case sf::Keyboard::Left:
                        if (!level->isPaused()){
                            if (going_right)
                                going_right = false;
                            going_left = true;
                        } else {
                            // PAUSED
                            level->backOptionLeft();
                        }
                        break;
                    case sf::Keyboard::Right:
                        if (!level->isPaused()){
                            if (going_left)
                                going_left = false;
                            going_right = true;
                        } else {
                            // PAUSED
                            level->backOptionRight();
                        }
                        break;
                    case sf::Keyboard::Enter:
                        if (level->isPaused()){
                            if (level->backOptionConfirm()){
                                // Go back to main menu
                                mainMenu = new MainMenu();
                                delete level;
                                is_main_menu = true;
                            }
                        } else {
                            level->shootBall();
                        }
                        break;
                    case sf::Keyboard::Space:
                        if (!level->isPaused()){
                            level->shootBall();
                        }
                }
            }
            else if (event.type == sf::Event::KeyReleased){
                switch (event.key.code){
                    case sf::Keyboard::Left:
                        going_left = false;
                    case sf::Keyboard::Right:
                        going_right = false;
                }
            }
        }
    }
}

void Game::initMainMenu() {
    mainMenu = new MainMenu();
}

void Game::updateConfigFile() {

}

