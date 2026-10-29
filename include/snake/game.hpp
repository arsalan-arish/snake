#pragma once

#include <snake/types.h>
#include <snake/sprites/snake.hpp>
#include <snake/sprite.hpp>
#include <vector>
#include <memory>

using std::vector;
using std::pair;
using std::unique_ptr;


class Game
{
public:
    Game();
    ~Game();

    void run();

private:

    /* Game loop callbacks */
    void event();   // Must be lightweight 
    void update();  // Does the heavylifting compute
    void render();  // Graphics Pipeline
};