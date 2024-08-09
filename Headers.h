//
// Created by sybilvane on 5/8/24.
//

#ifndef INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_HEADERS_H
#define INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_HEADERS_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <SFML/Window/Event.hpp>
#include <filesystem>
#include <fstream>
#include <cmath>

enum MOVEMENT {
    LEFT,
    RIGHT
};

enum BLOCK_TYPE {
    LIGHT,
    MEDIUM,
    HEAVY
};

static const int LIGHT_HEALTH = 1;
static const int MEDIUM_HEALTH = 3;
static const int HEAVY_HEALTH = 6;

static float master_volume = 20;

static const size_t window_width = 1400;
static const size_t window_height = 900;
static std::string config_file = "/home/sybilvane/Documents/breakout_config.txt";

#endif //INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_HEADERS_H
