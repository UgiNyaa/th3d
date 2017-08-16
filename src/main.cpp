#include "game.hpp"
#include "games/testgame/testgame.hpp"

int main(int argc, char *argv[])
{
    TestGame test;
    test.initialize(argc, argv);
    test.loop();
    test.deinitialize();
    exit(1);
}
