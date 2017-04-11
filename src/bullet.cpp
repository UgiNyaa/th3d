#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <bullet.hpp>

Bullet::Bullet(const Time& t, const Shape* const shape)
    : t(t)
    , shape(shape)
{
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
