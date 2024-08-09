//
// Created by sybilvane on 5/8/24.
//

#ifndef INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_MAINMENU_H
#define INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_MAINMENU_H

#include "Headers.h"

static const unsigned int title_size = 30;
static const unsigned int options_size = 20;

class MainMenu {
private:
    sf::Font font;
    sf::Text title;
    sf::Music menu_music;
    std::vector<sf::Texture> gif_frames;
    std::vector<sf::Text> options;
    std::vector<sf::Text> submenu_items;
    sf::Sprite gif_sprite;
    sf::Clock frame_clock;
    std::wstring option_text[5];
    std::wstring default_title_string;
    float title_x_pos;
    float title_y_pos;
    float menu_y_pos;
    float padding;
    float frame_duration = .12f;
    unsigned short selected_ind;
    unsigned short min_ind;
    unsigned short max_ind;
    unsigned short current_frame;
    unsigned short save_code;
    bool save_exists;
    bool in_submenu;
    bool game_beat;

    sf::SoundBuffer select;
    sf::SoundBuffer confirm;
    sf::Sound sound;

    void initFont();
    void initText();
    void initSoundsAndMusic();
    void initBackground();
    void initSelection();

    void recenter(sf::Text& text, std::wstring new_text, sf::Color color = sf::Color::White);
    void recenter(const std::wstring& new_text);
    std::string format_time_played(unsigned int time);
public:
    MainMenu();
    ~MainMenu();

    void goUp();
    void goDown();
    void goBack();
    int doAction();

    void update();
    void render(sf::RenderTarget& renderTarget);
};

#endif //INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_MAINMENU_H
