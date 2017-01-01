#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <utils.hpp>
#include <camera.hpp>
#include <shapes/shape.hpp>
#include <bmreader.hpp>

/* ---------------- MEMBERS ---------------- */
GLFWwindow* window;
GLuint vertexarrayID;
Player* player;
Camera* camera;
std::vector<Bullet*> bullets;
bool open = true;

/* ---------------- CALLBACKS ---------------- */
void scroll_callback
(
    GLFWwindow* window,
    double xoffset,
    double yoffset
);

/* ---------------- GAME LOOP METHODS ---------------- */
void update(float deltatime);
void draw(float deltatime);

/* ---------------- MAIN ---------------- */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Not enough args\n");
        return -1;
    }


    /* ---------------- GLFW INITIALIZATION ---------------- */
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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetScrollCallback(window, scroll_callback);

    /* ---------------- SHAPES INITIALIZATION ---------------- */
    Shape* shapes[] =
    {
        new Cube()
    };

    /* ---------------- BULLET MAP INITIALIZATION ---------------- */
    std::ifstream ifs(argv[1]);
    std::string str
    (
        (std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>()
    );
    bullets = bm_json_read
    (
        str.c_str(), 
        shapes,
        sizeof(shapes)/sizeof(shapes[0])
    );

    /* ---------------- COMPONENTS INITIALIZATION ---------------- */
    player = new Player(window, shapes[0], bullets);
    camera = new Camera(player);

    /* ---------------- GL INITIALIZATION ---------------- */
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &vertexarrayID);
    glBindVertexArray(vertexarrayID);

    //loadBMP_custom("resources/textures/miku.bmp");
    loadPNG("resources/textures/kawaii.png");

    /* ---------------- GAME LOOP ---------------- */
    auto lastTime = glfwGetTime();

    while (open)
    {
        auto dt = float(glfwGetTime() - lastTime);
        lastTime = glfwGetTime();
        update(dt);
        draw(dt);
    }

    /* ---------------- TERMINATION ---------------- */
    glDeleteVertexArrays(1, &vertexarrayID);
    glfwTerminate();
    glfwDestroyWindow(window);

    delete camera;
    delete player;

    for (auto bullet : bullets)
        delete bullet;

    for (auto shape : shapes)
        delete shape;

    return 0;
}

void update(float deltatime)
{
    player->update(deltatime);
    camera->update(deltatime);
    for (auto bullet : bullets)
        bullet->update(deltatime);

    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2, height/2);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
        open = false;
}

void draw(float deltatime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = camera->projection();
    auto view = camera->view();

    for (auto bullet : bullets)
        bullet->draw(view, projection);
    player->draw(view, projection);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void scroll_callback
(
    GLFWwindow* window,
    double xoffset,
    double yoffset
)
{
    camera->on_scroll(window, xoffset, yoffset);
}
