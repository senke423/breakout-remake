//
// Created by sybilvane on 5/8/24.
//

#include "MainMenu.h"

MainMenu::MainMenu() : padding(70), min_ind(0), max_ind(4), in_submenu(false),
current_frame(0), selected_ind(1), default_title_string(L"Breakout, by Senke!") {

    option_text[0] = L"Nastavi igru";
    option_text[1] = L"Nova igra";
    option_text[2] = L"Statistika";
    option_text[3] = L"Zasluge";
    option_text[4] = L"Zatvori";

    initFont();
    initText();
    initSoundsAndMusic();
    initBackground();
    initSelection();
}

MainMenu::~MainMenu() = default;

void MainMenu::update() {

}


void MainMenu::initFont() {
    font.loadFromFile("../res/Fonts/PressStart2P-Regular.ttf");
}

void MainMenu::initText() {
    title.setFont(font);
    title.setFillColor(sf::Color::White);
    title.setCharacterSize(title_size);
    title.setString(default_title_string);
    title_x_pos = ((float)window_width / 2) - ((float)title.getString().getSize() * (float)title_size)/2;
    title_y_pos = 200;
    title.setPosition(title_x_pos, title_y_pos);

    menu_y_pos = title_y_pos + 150;

    for (int i = 0; i < 5; i++){
        sf::Text option;
        option.setFont(font);
        if (i == 0 && !save_exists)
            option.setFillColor(sf::Color(128, 128, 128));
        else
            option.setFillColor(sf::Color::White);
        option.setCharacterSize(options_size);
        option.setString(option_text[i]);
        option.setPosition(((float)window_width / 2) - ((float)option_text[i].length() * (float)options_size)/2,
                           menu_y_pos + padding*i);

        options.push_back(option);
    }

}

void MainMenu::render(sf::RenderTarget &renderTarget) {

    if (frame_clock.getElapsedTime().asSeconds() > frame_duration){
        frame_clock.restart();
        current_frame = (current_frame + 1) % gif_frames.size();
        gif_sprite.setTexture(gif_frames[current_frame]);
        gif_sprite.setScale(2.59f, 1.8f);
    }
    renderTarget.draw(gif_sprite);

    if (!in_submenu){
        renderTarget.draw(title);
        for (const auto& i : options){
            renderTarget.draw(i);
        }
    } else {
        // submenu
        renderTarget.draw(title);
        for (const auto& i : submenu_items){
            renderTarget.draw(i);
        }
    }
}

void MainMenu::initBackground() {
    sf::Image gif_image;
    gif_image.loadFromFile("../res/Sprites/animated_background.png");

    // dimension of one frame: 540x540
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            sf::Texture frame;
            frame.loadFromImage(gif_image, sf::IntRect(x * 540, y * 540,
                                                       540, 540));
            gif_frames.push_back(frame);
        }
    }

    gif_sprite.setTexture(gif_frames[0]);
    gif_sprite.setScale(2.59f, 1.8f);
}

void MainMenu::initSoundsAndMusic() {
    menu_music.openFromFile("../res/Music/main_menu_music.ogg");
    menu_music.setVolume(master_volume);
    menu_music.play();

    select.loadFromFile("../res/Sounds/select1.wav");
    confirm.loadFromFile("../res/Sounds/confirm1.wav");

    sound.setVolume(master_volume);
}

void MainMenu::initSelection() {
    std::ifstream file(config_file);
    if (!file.is_open()){
        std::cout << "Failed to open file!";
        return;
    }

    std::string line;
    while(std::getline(file, line)){
        if(line[0] == '#') continue;

        if (line.substr(0, 10) == "last_save:"){
            std::string temp = line.substr(10);
            save_code = std::stoi(temp);
            if (save_code == 0)
                save_exists = false;
            else
                save_exists = true;
        }
        else if (line.substr(0, 10) == "game_beat:"){
            if (line.substr(10) == "true")
                game_beat = true;
            else
                game_beat = false;
        }
    }

    if (save_exists)
        selected_ind = 0;

    std::wstring temp = options.at(selected_ind).getString();
    temp = L"> " + temp + L" <";
    recenter(options.at(selected_ind), temp, sf::Color::Cyan);
}

void MainMenu::goUp() {
    if (in_submenu) return;

    if (selected_ind == min_ind) return;
    if (selected_ind == 1 && !save_exists) return;

    // reset selected index
    recenter(options.at(selected_ind), option_text[selected_ind]);

    selected_ind--;

    std::wstring temp = options.at(selected_ind).getString();
    temp = L"> " + temp + L" <";
    recenter(options.at(selected_ind), temp, sf::Color::Cyan);

    sound.setBuffer(select);
    sound.play();
}

void MainMenu::goDown(){
    if (in_submenu) return;

    if (selected_ind == max_ind) return;

    // reset selected index
    recenter(options.at(selected_ind), option_text[selected_ind]);

    selected_ind++;

    std::wstring temp = options.at(selected_ind).getString();
    temp = L"> " + temp + L" <";
    recenter(options.at(selected_ind), temp, sf::Color::Cyan);

    sound.setBuffer(select);
    sound.play();
}

void MainMenu::recenter(sf::Text &text, std::wstring new_text, sf::Color color) {
    text.setString(new_text);
    text.setFillColor(color);
    text.setPosition(((float)window_width / 2) - ((float)text.getString().getSize() * (float)options_size)/2,
                              menu_y_pos + padding*selected_ind);
}



int MainMenu::doAction() {
    if (in_submenu) return 100;

    std::wstring stats_text[3] = {L"Blokova uništeno:", L"Vreme igranja:", L"Izgubljeno života:"};
    std::wstring credits[] = {L"Pozadinska muzika:", L"moodmode (sajt 'pixabay.com')",
                              L"Zvučni efekti", L"freesound.org", L"Pozadinske slike:",
                              L"pinterest.com"};
    std::ifstream file(config_file);
    std::string line;
    unsigned int blocks_destroyed = 0;
    unsigned int time_played = 0;
    unsigned int lives_lost = 0;

    switch (selected_ind){
        case 0:
            // Nastavi
            return 1;
            break;
        case 1:
            // Nova igra
            return 2;
            break;
        case 2:
            // Statistika
            in_submenu = true;
            recenter(L"Statistika");

            for (int i = 0; i < 3; i++){
                sf::Text temp;
                temp.setFont(font);
                temp.setFillColor(sf::Color::White);
                temp.setCharacterSize(options_size);
                temp.setString(stats_text[i]);
                temp.setPosition(300, title_y_pos + 150 + padding*i);

                submenu_items.push_back(temp);
            }


            if (!file.is_open()){
                std::cout << "Failed to open file!";
                return false;
            }

            while(std::getline(file, line)){
                if (line.substr(0, 17) == "blocks_destroyed:")
                    blocks_destroyed = std::stoi(line.substr(17));
                else if (line.substr(0, 12) == "time_played:")
                    time_played = std::stoi(line.substr(12));
                else if (line.substr(0, 11) == "lives_lost:")
                    lives_lost = std::stoi(line.substr(11));
            }

            for (int i = 0; i < 3; i++){
                sf::Text temp;
                temp.setFont(font);
                temp.setFillColor(sf::Color::White);
                temp.setCharacterSize(options_size);
                switch (i){
                    case 0:
                        temp.setString(std::to_string(blocks_destroyed));
                        break;
                    case 1:
                        temp.setString(format_time_played(time_played));
                        break;
                    case 2:
                        temp.setString(std::to_string(lives_lost));
                        break;
                }
                temp.setPosition((float)window_width - 300 - (float)temp.getString().getSize() * options_size,
                                 title_y_pos + 150 + padding*i);

                submenu_items.push_back(temp);
            }
            return 3;
        case 3:
            // Zasluge
            in_submenu = true;
            recenter(L"Zasluge");

            for (int i = 0; i < 6; i += 2){
                sf::Text temp;
                temp.setFont(font);
                temp.setFillColor(sf::Color::White);
                temp.setCharacterSize(options_size);
                temp.setString(credits[i]);
                temp.setPosition(200, title_y_pos + 150 + padding*i/2);

                sf::Text temp2;
                temp2.setFont(font);
                temp2.setFillColor(sf::Color::White);
                temp2.setCharacterSize(options_size);
                temp2.setString(credits[i + 1]);
                temp2.setPosition((float)window_width - 200 - (float)temp2.getString().getSize() * options_size,
                                  title_y_pos + 150 + padding*i/2);

                submenu_items.push_back(temp);
                submenu_items.push_back(temp2);
            }
            return 4;
        case 4:
            // Zatvori
            return 5;
    }
}

void MainMenu::goBack() {
    if (in_submenu){
        recenter(default_title_string);
        in_submenu = false;
        submenu_items.clear();
        sound.setBuffer(confirm);
        sound.play();
    }
}

void MainMenu::recenter(const std::wstring& new_text) {
    title.setString(new_text);
    title.setPosition(((float)window_width / 2) - ((float)title.getString().getSize() * (float)title_size)/2,
                            title_y_pos);
}

std::string MainMenu::format_time_played(unsigned int time) {
    // time is in SECONDS
    unsigned int hrs, mins, secs;
    hrs = time/3600;
    mins = (time%3600)/60;
    secs = time%60;
    std::string temp;

    if (time < 60)
        temp = std::to_string(secs) + "s";
    else if (time < 3600)
        temp = std::to_string(mins) + "min " + std::to_string(secs) + "s";
    else
        temp = std::to_string(hrs) + "h " + std::to_string(mins) + "min " + std::to_string(secs) + "s";

    return temp;
}
