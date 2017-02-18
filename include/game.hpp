#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <time.hpp>

class Game
{
protected:
    Time t;
    bool open = false;

    GLFWwindow* window;
    GLuint vertexarrayID;

public:
    Game() { };
    virtual ~Game();

    virtual void initialize();
    virtual void update();
    virtual void draw();
    virtual void deinitialize();

    void loop();
};

#endif /* end of include guard: GAME_HPP */
