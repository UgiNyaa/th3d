#ifndef TESTGAME_HPP
#define TESTGAME_HPP

#include <game.hpp>
#include <time.hpp>
#include <player.hpp>
#include <camera.hpp>
#include <bmap.hpp>
#include <bullet.hpp>

#include <drawers/cubedrawer.hpp>

#include <shapes/shape.hpp>
#include <shapes/cubeshape.hpp>

class TestGame
    : public Game
{
protected:
    const std::vector<Shape*> shapes;

    Player* player;
    Camera* camera;

    BMap bmap;

    CubeDrawer cubedrawer;
    BoxCollider testbox;

    static void scroll_callback
    (
        GLFWwindow* window,
        double xoffset,
        double yoffset
    );

public:
    TestGame();

    virtual void initialize(int argc, char *argv[]) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void deinitialize() override;
};

#endif /* end of include guard: TESTGAME_HPP */
