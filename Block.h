//
// Created by sybilvane on 5/10/24.
//

#ifndef INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_BLOCK_H
#define INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_BLOCK_H

#include "Headers.h"
#include <random>

class Block {
    sf::Texture glass_texture;
    sf::Texture wood_texture;
    sf::Texture metal_texture;
    sf::Sprite sprite;
    sf::Sound sound;
    std::vector<sf::SoundBuffer> glass_sounds;
    std::vector<sf::SoundBuffer> wood_sounds;
    std::vector<sf::SoundBuffer> metal_sounds;
    BLOCK_TYPE type;
    float x;
    float y;
    float block_height;
    float block_width;
    int health;

    void initTexture();
    void initSounds();

public:

    Block(BLOCK_TYPE type, float x, float y);
    ~Block();

    bool downgrade();
    void render(sf::RenderTarget &renderTarget);
    sf::Vector2f getCoords();
    float getX();
    float getY();
    float getWidth();
    float getHeight();
};


#endif //INC_37_BREAKOUT_REMAKE_ATTEMPT2_CPP_BLOCK_H
