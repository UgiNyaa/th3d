#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <bmap.hpp>

#include <drawers/drawer.hpp>
#include <drawers/cubedrawer.hpp>

#include <colliders/collider.hpp>
#include <colliders/boxcollider.hpp>

GLuint cpID;
GLuint ssbo;
int shader_data[100];

/* ---------------- MEMBERS ---------------- */
const std::vector<Shape*> shapes =
{
    new CubeShape()
};

GLFWwindow* window;
GLuint vertexarrayID;
Player* player;
Camera* camera;
BMap bmap;
bool open = true;

CubeDrawer cubedrawer;

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
    cubedrawer.initialize();
    for (auto shape : shapes)
        shape->initialize();

    /* ---------------- BULLET MAP INITIALIZATION ---------------- */
    std::ifstream ifs(argv[1]);
    std::string str
    (
        (std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>()
    );
    bmap = BMap::from_json_file(str, shapes);

    /* ---------------- COMPONENTS INITIALIZATION ---------------- */
    player = new Player(window, shapes[0]);
    camera = new Camera(player);

    /* ---------------- COMPUTE SHADER TESTING FIELD ---------------- */
    // glGenBuffers(1, &ssbo);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    // glBufferData
    // (
    //     GL_SHADER_STORAGE_BUFFER,
    //     sizeof(shader_data),
    //     &shader_data,
    //     GL_DYNAMIC_COPY
    // );
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    // cpID = glCreateProgram();
    // auto cs = glCreateShader(GL_COMPUTE_SHADER);
    // GLuint block_index = glGetProgramResourceIndex(cpID, GL_SHADER_STORAGE_BLOCK, "Output");
    // glShaderStorageBlockBinding(cpID, block_index, 0);
    // const char* csSrc = R"(
    //     #version 430
    //     layout (local_size_x = 1, local_size_y = 1) in;
    //     layout (std430) buffer;
    //     layout(binding = 1) writeonly buffer Output
    //     {
    //         uint elements[];
    //     } output_data;
    //     void main()
    //     {
    //         uint ident = gl_GlobalInvocationID.x;
    //         output_data.elements[ident] = 5 + ident;
    //     }
    // )";
    // glShaderSource(cs, 1, &csSrc, NULL);
    // glCompileShader(cs);
    // int rvalue;
    // glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
    // if (!rvalue) {
    //     fprintf(stderr, "Error in compiling the compute shader\n");
    //     GLchar log[10240];
    //     GLsizei length;
    //     glGetShaderInfoLog(cs, 10239, &length, log);
    //     fprintf(stderr, "Compiler log:\n%s\n", log);
    //     exit(40);
    // }
    // glAttachShader(cpID, cs);
    // glLinkProgram(cpID);

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

    for (auto shape : shapes)
        delete shape;

    return 0;
}

BoxCollider testbox = BoxCollider(glm::vec3(5.0f, 1.0f, 5.0f));
void update(float deltatime)
{
    // glUseProgram(cpID);
    // glDispatchCompute(100, 1, 1);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    // GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    // memcpy(&shader_data, p, sizeof(shader_data));
    // glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    // for (size_t i = 0; i < 100; i++) {
    //     std::cout << shader_data[i] << '\n';
    // }
    // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    bmap.update(deltatime);
    player->update(deltatime);
    camera->update(deltatime);

    player->position += testbox.correct
    (
        glm::vec3(0.0f, -5.0f, 0.0f),
        static_cast<BoxCollider const&>(player->get_collider()),
        player->get_position(),
        player->get_velocity()
    );

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

    for (auto bullet : bmap.Bullets)
    {
        auto intersects = bullet->intersects(player);
        bullet->draw
        (
            view,
            projection,
            intersects ? glm::vec3(1.0f, 0.3f, 0.3f) : glm::vec3(1.0f)
        );
    }
    cubedrawer.draw(player->model(), view, projection, glm::vec3(1.0f));
    // player->draw(view, projection, glm::vec3(1.0f));

    // drawing the collider cube
    cubedrawer.draw
    (
        glm::translate(glm::vec3(0.0f, -5.0f, 0.0f)) * glm::scale(glm::vec3(5.0f, 1.0f, 5.0f)),
        view,
        projection,
        glm::vec3(0.3f, 0.3f, 1.0f)
    );

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
