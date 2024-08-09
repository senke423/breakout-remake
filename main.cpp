#include "Game.h"

/*
 * TO-DO:
 *
 * - implement collision with paddle
 */

int main() {
    Game game;

    // game loop
    while (game.getWindow().isOpen()){
        game.update();
        game.render();
    }

    return 0;
}
