//
// Created by sybilvane on 5/10/24.
//

#include "Block.h"

Block::Block(BLOCK_TYPE type, float x, float y) : block_height(64), block_width(128) {
    this->type = type;
    initTexture();
    initSounds();

    switch (type){
        case LIGHT:
            health = LIGHT_HEALTH;
            this->type = BLOCK_TYPE::LIGHT;
            sprite.setTexture(glass_texture);
            break;
        case MEDIUM:
            health = MEDIUM_HEALTH;
            this->type = BLOCK_TYPE::MEDIUM;
            sprite.setTexture(wood_texture);
            break;
        case HEAVY:
            health = HEAVY_HEALTH;
            this->type = BLOCK_TYPE::HEAVY;
            sprite.setTexture(metal_texture);
            break;
    }

    sound.setVolume(master_volume);

    this->x = x;
    this->y = y;
    sprite.setPosition(this->x, this->y);
    sprite.setScale(2, 2);
}

Block::~Block() {

}

// FALSE -> WASN'T DESTROYED
// TRUE -> WAS DESTROYED
bool Block::downgrade() {

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution1(1,(int)glass_sounds.size());
    std::uniform_int_distribution<int> distribution2(1, 100);
    std::uniform_int_distribution<int> distribution3(1, (int)metal_sounds.size());
    int dice_roll;

    switch (type){

        case LIGHT:
            dice_roll = distribution1(generator);
            sound.setBuffer(glass_sounds[dice_roll - 1]);
            break;
        case MEDIUM:
            dice_roll = distribution2(generator);
            // probability of wood1_long.wav is 10%. For the other three: 30%.
            if (dice_roll <= 10)
                sound.setBuffer(wood_sounds[0]);
            else if (dice_roll <= 40)
                sound.setBuffer(wood_sounds[1]);
            else if (dice_roll <= 70)
                sound.setBuffer(wood_sounds[2]);
            else
                sound.setBuffer(wood_sounds[3]);
            break;
        case HEAVY:
            dice_roll = distribution3(generator);
            sound.setBuffer(metal_sounds[dice_roll - 1]);
            break;
    }
    sound.play();
    health--;

    if (type == BLOCK_TYPE::LIGHT && health == 0)
        return true;

    else if (type == BLOCK_TYPE::MEDIUM && health == 0){
        type = BLOCK_TYPE::LIGHT;
        sprite.setTexture(glass_texture);
        health = LIGHT_HEALTH;
    }

    else if (type == BLOCK_TYPE::HEAVY && health == 0){
        type = BLOCK_TYPE::MEDIUM;
        sprite.setTexture(wood_texture);
        health = MEDIUM_HEALTH;
    }
    return false;
}

void Block::initTexture() {
    glass_texture.loadFromFile("../res/Sprites/glass.png");
    wood_texture.loadFromFile("../res/Sprites/wood.png");
    metal_texture.loadFromFile("../res/Sprites/metal.png");
}

void Block::render(sf::RenderTarget &renderTarget) {
    renderTarget.draw(sprite);
}

float Block::getWidth() {
    return block_width;
}

float Block::getHeight() {
    return block_height;
}

sf::Vector2f Block::getCoords() {
    return {x, y};
}

float Block::getX() {
    return x;
}

float Block::getY() {
    return y;
}

void Block::initSounds() {
    sf::SoundBuffer temp_buffer;
    if (!temp_buffer.loadFromFile("../res/Sounds/glass1.wav"))
        std::cout << "Couldn't load glass1.wav sound! (Block.cpp)\n";
    else
        glass_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/glass2.wav"))
        std::cout << "Couldn't load glass2.wav sound! (Block.cpp)\n";
    else
        glass_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/glass3.wav"))
        std::cout << "Couldn't load glass3.wav sound! (Block.cpp)\n";
    else
        glass_sounds.emplace_back(temp_buffer);


    if (!temp_buffer.loadFromFile("../res/Sounds/metal1.wav"))
        std::cout << "Couldn't load metal1.wav sound! (Block.cpp)\n";
    else
        metal_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/metal2.wav"))
        std::cout << "Couldn't load metal2.wav sound! (Block.cpp)\n";
    else
        metal_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/metal3.wav"))
        std::cout << "Couldn't load metal3.wav sound! (Block.cpp)\n";
    else
        metal_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/metal4.wav"))
        std::cout << "Couldn't load metal4.wav sound! (Block.cpp)\n";
    else
        metal_sounds.emplace_back(temp_buffer);


    if (!temp_buffer.loadFromFile("../res/Sounds/wood1_long.wav"))
        std::cout << "Couldn't load wood1_long.wav sound! (Block.cpp)\n";
    else
        wood_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/wood2_short.wav"))
        std::cout << "Couldn't load wood2_short.wav sound! (Block.cpp)\n";
    else
        wood_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/wood3.wav"))
        std::cout << "Couldn't load wood3.wav sound! (Block.cpp)\n";
    else
        wood_sounds.emplace_back(temp_buffer);
    if (!temp_buffer.loadFromFile("../res/Sounds/wood4.wav"))
        std::cout << "Couldn't load wood4.wav sound! (Block.cpp)\n";
    else
        wood_sounds.emplace_back(temp_buffer);
}
