#ifndef TESTGAME_HPP
#define TESTGAME_HPP

#include <memory>
#include <map>
#include <string>

#include <glm/gtx/norm.hpp>

#include <lua.hpp>

#include "../../shape.hpp"
#include "../../game.hpp"
#include "../../player.hpp"
#include "../../camera.hpp"
#include "../../bullet.hpp"
#include "../../unit.hpp"
#include "../../entity.hpp"

class TestGame
    : public Game
{
protected:
    lua_State* L;

    Shape s;
    std::map<std::string, Shape*> shapes;

    Player player;
    Camera camera;

    std::vector<Entity*> entities;

    std::unique_ptr<Shape> player_shape;
    BoxCollider testbox;

    void update_player();
    void update_entities();
    void update_entity();
    void update_pattern();

    glm::mat4 entity_model(const Entity& e);
    glm::mat4 bullet_model(const Bullet& b);
    glm::mat4 player_model();
    glm::mat4 camera_view();
    glm::mat4 camera_projection();

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
