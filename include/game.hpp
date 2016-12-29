#ifndef GAME_HPP
#define GAME_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <square.hpp>
#include <camera.hpp>
#include <bullet.hpp>
#include <player.hpp>
#include <vector>

class Game
{
private:
    GLFWwindow* window;
    GLuint vertexarrayID;
    Camera* camera;
    Square* square;
    Player* player;
    std::vector<Bullet*> bullets;
    bool open;
public:
    Game(std::vector<Bullet*> bullets);
    ~Game ();

    GLFWwindow* get_window();
    Camera* get_camera();
    Square* get_square();
    bool is_open();


    void initialize();
    void update(float deltatime);
    void draw(float deltatime);
};

#endif /* end of include guard: GAME_HPP */
