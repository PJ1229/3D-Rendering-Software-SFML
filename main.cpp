#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    Game game;

    while (game.getWindowIsOpen())
    {
        //update
        game.update();
        
        //render
        game.render();
    }

    return 0;
}