#include <iostream>
#include "Breakout.h"


int main()
{
    // Create the game (title, window size, target FPS)
    Breakout newGame("Breakout", constants::gScreenWidth, constants::gScreenHeight, 60);
    // Run the game loop
    newGame.run();
    
    return 0;
}