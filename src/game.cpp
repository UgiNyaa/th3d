#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <utils.hpp>
#include <camera.hpp>
#include <square.hpp>
#include <game.hpp>
#include <bmreader.hpp>


Game::Game(std::vector<Bullet*> bullets)
    : bullets(bullets)
{
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        throw "Failed to initialize GLFW\n";
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Tutorial 01", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials\n");
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials\n";
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        throw "Failed to initialize GLEW\n";
    }

    camera = new Camera(this);
    square = new Square();
    open = true;
}

Game::~Game()
{
    delete camera;
    delete square;

    glDeleteVertexArrays(1, &vertexarrayID);
    glfwTerminate();

    for (auto bullet : bullets)
        delete bullet;
}

GLFWwindow* Game::get_window() { return this->window; }
Camera* Game::get_camera() { return this->camera; }
Square* Game::get_square() { return this->square; }
bool Game::is_open() { return this->open;}

void Game::initialize()
{
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &vertexarrayID);
    glBindVertexArray(vertexarrayID);

    GLuint texture = loadBMP_custom("resources/textures/miku.bmp");
}

void Game::update(float deltatime)
{
    camera->update(deltatime);
    for (auto bullet : bullets)
        bullet->update(deltatime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
        open = false;
}

void Game::draw(float deltatime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = camera->projection();
    auto view = camera->view();

    for (auto bullet : bullets)
    {
        auto model = bullet->model();
        square->draw(model, view, projection);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}
