//
// Created by sybilvane on 5/8/24.
//

#include "Level.h"


void Level::update() {
    if (paused)
        return;

    if (!ball_dropped && ball->update(delta_time, paddle, blocks)){
        // BALL IS OUT OF SCREEN
        ball_dropped = true;

        sf::Vector2f ball_velocity;
        ball_velocity.x = 5;
        ball_velocity.y = -5 * acceleration_coeff;
        ball->setVelocity(ball_velocity);
        sound.setBuffer(life_lost);
        sound.play();
        countdown_clock.restart();
    }

    if (ball->getFollowPaddle())
        return;

    // check if ball collided with paddle
    // check if center of ball is inside the borders of the paddle
    if (ball->getX() + ball->getWidth()/2 > paddle->getX() &&
        ball->getX() + ball->getWidth()/2 < paddle->getX() + paddle->getWidth() &&
        ball->getY() + ball->getHeight() > paddle->getY() &&
        ball->getY() + ball->getHeight() < paddle->getY() + paddle->getHeight() &&
        ball->getVelocity().y > 0){

        ball->reverseYVelocity();
        sound.setBuffer(paddle_hit);
        sound.play();
    }

    // check if ball collided with a brick
    for (auto& i : blocks){
        bool left, right, top, bottom;
        left = right = top = bottom = false;

        // x -> start point, y -> end point
//        sf::Points velocity_vector = {sf::Vector2f(), sf::Vector2f()};
//        sf::Vector2f temp = {ball->getX() + ball->getWidth()/2, ball->getY() + ball->getHeight()/2};
//        velocity_vector.x = temp;


        if (left || right || top || bottom){
            score += 10;
            if (i->downgrade()){
                blocks.erase(std::remove(blocks.begin(), blocks.end(), i), blocks.end());
                blocks_destroyed++;
                score += 50;
                std::cout << "Blocks destroyed: " << blocks_destroyed << "\n";
            }

            break;
        }
    }

    hud.at(1).setString("score: " + std::to_string(score));
}

void Level::render(sf::RenderTarget &renderTarget) {

    if (frame_clock.getElapsedTime().asSeconds() > frame_duration){
        frame_clock.restart();
        current_frame = (current_frame + 1) % gif_frames.size();
        gif_sprite.setTexture(gif_frames[current_frame]);
        gif_sprite.setScale(1.863f, 1.86f);
    }
    renderTarget.draw(gif_sprite);


    paddle->render(renderTarget);
    ball->render(renderTarget);

    // dark bar at the bottom
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(window_width, 60));
    rect.setPosition(0, 840);
    rect.setFillColor(sf::Color(0, 0, 0, 180));
    renderTarget.draw(rect);
    // ----------------------

    for (const auto& i : hud){
        renderTarget.draw(i);
    }
    for (const auto& i : blocks){
        i->render(renderTarget);
    }

    if (game_over){
        sound.setBuffer(game_over_sound);
        sound.play();
    }

    // dark bar across the middle of the screen
    // when there's a countdown or the ball is dropped
    if (countdown || ball_dropped){
        sf::RectangleShape rect2;
        rect2.setSize(sf::Vector2f(window_width, 80));
        rect2.setPosition(0, 410);
        rect2.setFillColor(sf::Color(0, 0, 0, 180));
        renderTarget.draw(rect2);
    }

    if (countdown){
        float get_ready_time = 1.5;
        if (paused){}
        else if (countdown_clock.getElapsedTime().asSeconds() + elapsed_countdown <= get_ready_time){
            recenter(L"Spremi se!");
        }
        else if (countdown_clock.getElapsedTime().asSeconds() + elapsed_countdown <= get_ready_time + 1){
            recenter(L"3");
            if (!countdown_flags[0]){
                sound.setBuffer(beep2);
                sound.play();
                countdown_flags[0] = true;
            }
        }
        else if (countdown_clock.getElapsedTime().asSeconds() + elapsed_countdown <= get_ready_time + 2){
            recenter(L"2");
            if (!countdown_flags[1]){
                sound.setBuffer(beep2);
                sound.play();
                countdown_flags[1] = true;
            }
        }
        else if (countdown_clock.getElapsedTime().asSeconds() + elapsed_countdown <= get_ready_time + 3){
            recenter(L"1");
            if (!countdown_flags[2]){
                sound.setBuffer(beep2);
                sound.play();
                countdown_flags[2] = true;
            }
        }
        else if (countdown_clock.getElapsedTime().asSeconds() + elapsed_countdown <= get_ready_time + 3.5){
            recenter(L"Kreni!");
            if (!countdown_flags[3]){
                sound.setBuffer(beep3);
                sound.play();
                countdown_flags[3] = true;
                ball->setFollowPaddle(false);
                sound.setBuffer(shooting_sound);
                sound.play();
            }
        }
        else if (countdown_clock.getElapsedTime().asSeconds() + elapsed_countdown <= 6.2){
            countdown = false;
        }

        if (!ball_dropped)
            renderTarget.draw(message);
    }

    if (ball_dropped){
        if (countdown_clock.getElapsedTime().asSeconds() <= 1){
            recenter(L"-1 život");
            renderTarget.draw(message);
        } else {
            countdown_flags[0] = false;
            countdown_flags[1] = false;
            countdown_flags[2] = false;
            countdown_flags[3] = false;
//            countdown = true; -> no countdown when ball's dropped
            ball->setFollowPaddle(true);
            ball_dropped = false;
            lives_lost++;
            lives--;
            countdown_clock.restart();

            if (lives == 0){
                game_over = true;
                ball_dropped = false;
            } else {
                // hud : FPS, SCORE, LIVES
                hud.at(2).setString(L"života: " + std::to_wstring(lives));
            }

        }
    }

    if (paused){
        for (const auto& i : pause_text){
            renderTarget.draw(*i);
        }
    }
}

Level::Level() : elapsed_time(0), paused(false), countdown(true), delta_time(1), back_option(1),
                 blocks_destroyed(0), lives_lost(0), score(0), lives(3), message_size(40), elapsed_countdown(0),
                 paddle(nullptr), ball(nullptr), ball_dropped(false), game_over(false),
                 acceleration_coeff(1) {

    initBackground();

    paddle = new Paddle(400, 750);

    ball = new Ball(1, start_velocity.x, start_velocity.y);

    initFont();
    initSounds();
    initPauseScreen();
    initHud();
    initLevel();

    countdown_flags[0] = false;
    countdown_flags[1] = false;
    countdown_flags[2] = false;
    countdown_flags[3] = false;

    sound.setBuffer(confirm);
    sound.play();
    countdown_clock.restart();
    message.setCharacterSize(message_size);
    message.setFont(font);
    message.setFillColor(sf::Color::White);
    recenter(L"Spremi se!");
}

Level::~Level() {
    delete paddle;
    delete ball;

    for (auto i : pause_text){
        delete i;
    }
}

unsigned int Level::getElapsedTime() {
    return elapsed_time;
}

void Level::pause() {
    if (!paused){
        sound.setBuffer(pause_on_sound);
        sound.play();
        elapsed_time += std::round(stopwatch.getElapsedTime().asSeconds());
        paused = true;
        elapsed_countdown += countdown_clock.getElapsedTime().asSeconds();
    } else {
        sound.setBuffer(pause_off_sound);
        sound.play();
        stopwatch.restart();
        paused = false;
        back_option = 1;
        countdown_clock.restart();
        dynamic_cast<sf::Text*>(pause_text.at(3))->setFillColor(sf::Color::White);
        dynamic_cast<sf::Text*>(pause_text.at(4))->setFillColor(sf::Color::Cyan);
    }
}

void Level::movePaddleLeft() {
    if (!paused)
        paddle->update(MOVEMENT::LEFT, delta_time);
}

void Level::movePaddleRight() {
    if (!paused)
        paddle->update(MOVEMENT::RIGHT, delta_time);
}

void Level::setDeltaTime(float delta_time) {
    this->delta_time = delta_time;
}

void Level::initPauseScreen() {
    auto* rect = new sf::RectangleShape();
    rect->setSize(sf::Vector2f(window_width, window_height));
    rect->setPosition(0, 0);
    rect->setFillColor(sf::Color(0, 0, 0, 180));
    pause_text.emplace_back(rect);

    int pause_size = 30;
    float pause_x, pause_y;
    auto* temp = new sf::Text();
    temp->setFont(font);
    temp->setFillColor(sf::Color::White);
    temp->setCharacterSize(pause_size);
    temp->setString("PAUZIRANO");

    pause_x = ((float)window_width / 2) - ((float)temp->getString().getSize() * pause_size)/2;
    pause_y = ((float)window_height / 2) - pause_size/2 - 150;

    temp->setPosition(pause_x, pause_y);
    pause_text.emplace_back(temp);

    int text_size = 20;
    auto* temp2 = new sf::Text();
    temp2->setFont(font);
    temp2->setFillColor(sf::Color::White);
    temp2->setCharacterSize(text_size);
    temp2->setString(L"Da li želiš da izađeš?");
    temp2->setPosition(((float)window_width / 2) - ((float)temp2->getString().getSize() * text_size)/2,
                     pause_y + 75);
    pause_text.emplace_back(temp2);

    auto* temp3 = new sf::Text();
    temp3->setFont(font);
    temp3->setFillColor(sf::Color::White);
    temp3->setCharacterSize(text_size);
    temp3->setString(L"Da");
    temp3->setPosition(((float)window_width / 2) - ((float)temp3->getString().getSize() * text_size)/2 - text_size*3,
                      pause_y + 140);
    pause_text.emplace_back(temp3);

    auto* temp4 = new sf::Text();
    temp4->setFont(font);
    temp4->setFillColor(sf::Color::Cyan);
    temp4->setCharacterSize(text_size);
    temp4->setString(L"Ne");
    temp4->setPosition(((float)window_width / 2) - ((float)temp4->getString().getSize() * text_size)/2 + text_size*3,
                      pause_y + 140);
    pause_text.emplace_back(temp4);
}

void Level::initFont() {
    font.loadFromFile("../res/Fonts/PressStart2P-Regular.ttf");
}

bool Level::isPaused() const {
    return paused;
}

void Level::backOptionLeft() {
    // 'Da' -> at: 3
    // 'Ne' -> at: 4
    if (back_option == 1){
        dynamic_cast<sf::Text*>(pause_text.at(3))->setFillColor(sf::Color::Cyan);
        dynamic_cast<sf::Text*>(pause_text.at(4))->setFillColor(sf::Color::White);
        back_option = 0;
        sound.setBuffer(select);
        sound.play();
    }
}

void Level::backOptionRight() {
    if (back_option == 0){
        dynamic_cast<sf::Text*>(pause_text.at(3))->setFillColor(sf::Color::White);
        dynamic_cast<sf::Text*>(pause_text.at(4))->setFillColor(sf::Color::Cyan);
        back_option = 1;
        sound.setBuffer(select);
        sound.play();
    }
}

bool Level::backOptionConfirm() {
    sound.setBuffer(confirm);
    sound.play();
    if (dynamic_cast<sf::Text*>(pause_text.at(3))->getFillColor() == sf::Color::Cyan){
        // RETURN TO MAIN MENU
        return true;
    } else {
        // CONTINUE
        stopwatch.restart();
        paused = false;
        back_option = 1;
        countdown_clock.restart();
        return false;
    }
}

void Level::initSounds() {
    if (!select.loadFromFile("../res/Sounds/select1.wav"))
        std::cout << "Couldn't load select1.wav sound! (Level.cpp)\n";
    if (!confirm.loadFromFile("../res/Sounds/confirm1.wav"))
        std::cout << "Couldn't load confirm1.wav sound! (Level.cpp)\n";
    if (!beep2.loadFromFile("../res/Sounds/beep2.wav"))
        std::cout << "Couldn't load beep2.wav sound! (Level.cpp)\n";
    if (!beep3.loadFromFile("../res/Sounds/beep3.wav"))
        std::cout << "Couldn't load beep3.wav sound! (Level.cpp)\n";
    if (!life_lost.loadFromFile("../res/Sounds/life_lost.wav"))
        std::cout << "Couldn't load life_lost.wav sound! (Level.cpp)\n";
    if (!game_over_sound.loadFromFile("../res/Sounds/game_over2.wav"))
        std::cout << "Couldn't load game_over2.wav sound! (Level.cpp)\n";
    if (!pause_on_sound.loadFromFile("../res/Sounds/pause_on.wav"))
        std::cout << "Couldn't load pause_on.wav sound! (Level.cpp)\n";
    if (!pause_off_sound.loadFromFile("../res/Sounds/pause_off.wav"))
        std::cout << "Couldn't load pause_off.wav sound! (Level.cpp)\n";
    if (!paddle_hit.loadFromFile("../res/Sounds/hit6.wav"))
        std::cout << "Couldn't load hit6.wav sound! (Level.cpp)\n";
    if (!shooting_sound.loadFromFile("../res/Sounds/hit2.wav"))
        std::cout << "Couldn't load hit2.wav sound! (Level.cpp)\n";

    sound.setVolume(master_volume);
}

unsigned int Level::getBlocksDestroyed() {
    return blocks_destroyed;
}

int Level::getLivesLost() {
    return lives_lost;
}

void Level::initHud() {
    int text_size = 20;
    float padding = 20;

    sf::Text temp1;
    temp1.setFont(font);
    temp1.setFillColor(sf::Color::White);
    temp1.setCharacterSize(text_size);
    temp1.setString("0 fps");
    temp1.setPosition(padding, padding);

    hud.emplace_back(temp1);

    sf::Text temp2;
    temp2.setFont(font);
    temp2.setFillColor(sf::Color::White);
    temp2.setCharacterSize(text_size);
    temp2.setString("score: 0");
    temp2.setPosition(padding, window_height - padding - text_size);

    hud.emplace_back(temp2);

    sf::Text temp3;
    temp3.setFont(font);
    temp3.setFillColor(sf::Color::White);
    temp3.setCharacterSize(text_size);
    temp3.setString(L"života: 3");
    temp3.setPosition(window_width - padding - 9 * text_size, window_height - padding - text_size);

    hud.emplace_back(temp3);
}

void Level::setFps(unsigned int fps) {
    if (!paused){
        hud.at(0).setString(std::to_string(fps) + " fps");
    }
}

void Level::initLevel() {
    int i = 0;
    auto* block_test1 = new Block(BLOCK_TYPE::HEAVY, 60 + 128 * i++, 100);
    auto* block_test2 = new Block(BLOCK_TYPE::MEDIUM, 60 + 128 * i++, 100);
    auto* block_test3 = new Block(BLOCK_TYPE::LIGHT, 60 + 128 * i++, 100);
    auto* block_test4 = new Block(BLOCK_TYPE::HEAVY, 60 + 128 * i++, 100);
    auto* block_test5 = new Block(BLOCK_TYPE::LIGHT, 60 + 128 * i++, 100);
    auto* block_test6 = new Block(BLOCK_TYPE::MEDIUM, 60 + 128 * i++, 100);
    auto* block_test7 = new Block(BLOCK_TYPE::MEDIUM, 60 + 128 * i++, 100);
    auto* block_test8 = new Block(BLOCK_TYPE::LIGHT, 60 + 128 * i++, 100);
    auto* block_test9 = new Block(BLOCK_TYPE::MEDIUM, 60 + 128 * i++, 100);
    auto* block_test10 = new Block(BLOCK_TYPE::HEAVY, 60 + 128 * i++, 100);
    blocks.emplace_back(block_test1);
    blocks.emplace_back(block_test2);
//    blocks.emplace_back(block_test3);
//    blocks.emplace_back(block_test4);
    blocks.emplace_back(block_test5);
    blocks.emplace_back(block_test6);
    blocks.emplace_back(block_test7);
//    blocks.emplace_back(block_test8);
//    blocks.emplace_back(block_test9);
    blocks.emplace_back(block_test10);
}

void Level::recenter(std::wstring new_text) {
    message.setString(new_text);
    message.setPosition((window_width - message.getString().getSize() * message_size)/2,
                              window_height/2 - message_size/2);
}

void Level::initBackground() {
    sf::Image gif_image;
    gif_image.loadFromFile("../res/Sprites/animated_background2.png");

    // dimension of one frame: 752x484
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            sf::Texture frame;
            frame.loadFromImage(gif_image, sf::IntRect(x * 752, y * 484,
                                                       752, 484));
            gif_frames.push_back(frame);
        }
    }

    gif_sprite.setTexture(gif_frames[0]);
    gif_sprite.setScale(1.863f, 1.86f);
}

void Level::shootBall() {
    if (ball->getFollowPaddle()){
        ball->setFollowPaddle(false);
        ball->setVelocity(start_velocity);
        sound.setBuffer(shooting_sound);
        sound.play();
    }
}
