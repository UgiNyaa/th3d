#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <game.hpp>
#include <bullet.hpp>
#include <bmreader.hpp>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Not enough args\n");
        return -1;
    }

    std::ifstream ifs(argv[1]);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    auto bullets = bm_json_read(str.c_str());

    Game game = Game(bullets);
    game.initialize();

    auto lastTime = glfwGetTime();

    while (game.is_open())
    {
        auto dt = float(glfwGetTime() - lastTime);
        lastTime = glfwGetTime();
        game.update(dt);
        game.draw(dt);
    }

    return 0;
}
