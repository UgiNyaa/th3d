#include <iostream>
#include <vector>

#include <utils.hpp>

#include <games/testgame/testgame.hpp>

TestGame::TestGame()
    : shapes({ new CubeShape() })
    , player()
    , camera()
    , testbox(glm::vec3(5.0f, 1.0f, 5.0f))
{
}

void TestGame::initialize(int argc, char *argv[])
{
    Game::initialize(argc, argv);

    glfwSetScrollCallback(window, TestGame::scroll_callback);

    cubedrawer.initialize();
    for (auto shape : shapes)
        shape->initialize();

    std::ifstream ifs(argv[1]);
    std::string str
    (
        (std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>()
    );
    init_bmap(argv[1]);

    loadPNG("resources/textures/kawaii.png");
}

void TestGame::deinitialize()
{
    Game::deinitialize();

    for (auto shape : shapes)
        delete shape;

    for (auto b : processing_bullets)
        delete b;

    for (auto u : units)
    {
        for (auto b : u->bullets)
            delete b;
        for (auto p : u->patterns)
            delete p;
        delete u;
    }
}

void TestGame::update()
{
    update_player(t.delta_seconds());

    player.position += testbox.correct
    (
        glm::vec3(0.0f, -5.0f, 0.0f),
        player.box,
        player.position,
        player.velocity
    );

    for (auto u : units)
    {
        for (size_t i = 0; i < u->patterns.size(); i++)
        {
            float ux, uy, uz;
            u->pos(ux, uy, uz);
            u->patterns[i]->ux = ux;
            u->patterns[i]->uy = uy;
            u->patterns[i]->uz = uz;
            u->patterns[i]->start(processing_bullets);
        }

        if (u->patterns.size() > 0)
            u->patterns.erase(u->patterns.begin(), u->patterns.end());
    }

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

    for(auto u : units)
        u->shape->draw
        (
            unit_model(*u),
            view,
            projection,
            glm::vec3(0.6f, 1.0f, 1.0f)
        );

    for (auto b : processing_bullets)
    {
        if (b->start <= t.full)
            b->shape->draw
            (
                bullet_model(*b),
                view,
                projection,
                glm::vec3(0.3f, 1.0f, 0.3f)
            );
    }

    if (camera.distance != 0)
        cubedrawer.draw(player_model(), view, projection, glm::vec3(1.0f));
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
