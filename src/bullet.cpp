#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <bullet.hpp>

float gametime;

Bullet::Bullet(const Time& t, const Shape* const shape)
    : t(t)
    , shape(shape)
{
    this->position = glm::vec3(0, 0, 0);
    gametime = 0;

    f_position_x = [](float t) -> float{ return 0; };
    f_position_y = [](float t) -> float{ return 0; };
    f_position_z = [](float t) -> float{ return 0; };
}

bool Bullet::intersects(Player& player)
{
    glm::vec3 position;
    (*pos)(position.x, position.y, position.z, t.full_seconds());

    return player
        .box
        .intersects
        (
            player.position,
            shape->collider(),
            position
        );
}

void Bullet::update(float deltatime)
{
    gametime += deltatime;

    position.x = f_position_x(gametime);
    position.y = f_position_y(gametime);
    position.z = f_position_z(gametime);
}

void Bullet::draw
(
    glm::mat4 view,
    glm::mat4 projection,
    glm::vec3 colourmultiplier
)
{
    shape->draw(model(), view, projection, colourmultiplier);
}

glm::mat4 Bullet::model()
{
    glm::vec3 position;
    (*pos)(position.x, position.y, position.z, t.full_seconds());
    return glm::translate(position);
}
