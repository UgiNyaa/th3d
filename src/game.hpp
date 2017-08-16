#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "time.hpp"

#ifdef DEBUG
    #define D(x) x
#else
    #define D(x)
#endif

struct GLFWwindow
{
    class Game* game;
};

class Game
{
protected:
    Time t;
    bool open = false;

    GLFWwindow* window;

public:
    Game() { };
    virtual ~Game();

    virtual void initialize(int argc, char *argv[]);
    virtual void update();
    virtual void draw();
    virtual void deinitialize();

    void loop();
};

#endif /* end of include guard: GAME_HPP */
