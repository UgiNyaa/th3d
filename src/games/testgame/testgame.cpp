#include <iostream>
#include <vector>

#include "../../utils.hpp"

#include "testgame.hpp"

TestGame::TestGame()
    : s("resources/player.obj")
    , player()
    , camera()
    , testbox(glm::vec3(5.0f, 1.0f, 5.0f))
{
}

void TestGame::initialize(int argc, char *argv[])
{
    Game::initialize(argc, argv);

    glfwSetScrollCallback(window, TestGame::scroll_callback);

    init_bmap(argv[1]);

    player_shape = std::make_unique<Shape>("resources/player.obj");
    player_shape->initialize();
    for (auto shape : shapes)
        shape.second->initialize();
    s.initialize();

    loadPNG("resources/textures/kawaii.png");
}

void TestGame::deinitialize()
{
    Game::deinitialize();

    for (auto shape : shapes)
        delete shape.second;
    
    for (auto e : entities)
        delete e;
}

void TestGame::update()
{
    update_player();

    player.position += testbox.correct
    (
        glm::vec3(0.0f, -5.0f, 0.0f),
        player.box,
        player.position,
        player.velocity
    );

    update_entities();

    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2, height/2);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
        open = false;
}

void TestGame::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = camera_projection();
    auto view = camera_view();

    for(auto e : entities)
        e->shape->draw
        (
            entity_model(*e),
            view,
            projection,
            glm::vec3(0.6f, 1.0f, 1.0f)
        );

    if (camera.distance != 0)
        player_shape->draw(player_model(), view, projection, glm::vec3(1.0f));
    // player->draw(view, projection, glm::vec3(1.0f));

    // drawing the collider cube (currently with player shape)
    player_shape->draw
    (
        glm::translate(glm::vec3(0.0f, -5.0f, 0.0f)) * glm::scale(glm::vec3(5.0f, 1.0f, 5.0f)),
        view,
        projection,
        glm::vec3(0.3f, 0.3f, 1.0f)
    );

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void TestGame::scroll_callback
(
    GLFWwindow* window,
    double xoffset,
    double yoffset
)
{
    auto game = (TestGame*)window->game;
    game->camera.distance -= yoffset;
    if (game->camera.distance < 0)
        game->camera.distance = 0;
}
