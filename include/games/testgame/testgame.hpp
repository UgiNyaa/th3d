#ifndef TESTGAME_HPP
#define TESTGAME_HPP

#include <memory>
#include <map>
#include <string>

#include <glm/gtx/norm.hpp>

#include <shape.hpp>
#include <game.hpp>
#include <time.hpp>
#include <player.hpp>
#include <camera.hpp>
#include <bullet.hpp>
#include <unit.hpp>

#include <drawers/cubedrawer.hpp>

class TestGame
    : public Game
{
protected:
    std::map<std::string, Shape*> shapes;

    Player player;
    Camera camera;

    std::vector<Unit*> units;
    std::vector<Bullet*> processing_bullets;

    CubeDrawer cubedrawer;
    BoxCollider testbox;

    void update_player(float dt);

    glm::mat4 player_model();
    glm::mat4 camera_view();
    glm::mat4 camera_projection();
    glm::mat4 unit_model(const Unit& u);
    glm::mat4 bullet_model(const Bullet& b);

    static void scroll_callback
    (
        GLFWwindow* window,
        double xoffset,
        double yoffset
    );

    void init_bmap(std::string json);

public:
    TestGame();

    virtual void initialize(int argc, char *argv[]) override;
    virtual void update() override;
    virtual void draw() override;
    virtual void deinitialize() override;
};

#endif /* end of include guard: TESTGAME_HPP */
