#include <snake/program.h>
#include <snake/game.h>

int main(int argc, char* argv[])
{
    // Program Resources & Config managed by program class
    Program prog(argv);

    // Game Resources & Config managed by game class
    Game game;

    // Game Loop
    game.run();
}