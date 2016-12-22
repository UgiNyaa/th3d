#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <bullet.hpp>

float gametime;

Bullet::Bullet()
{
    this->position = glm::vec3(0, 0, 0);
    gametime = 0;

    f_position_x = [](float t) -> float{ return 0; };
    f_position_y = [](float t) -> float{ return 0; };
    f_position_z = [](float t) -> float{ return 0; };
}

void Bullet::update(float deltatime)
{
    gametime += deltatime;

    position.x = f_position_x(gametime);
    position.y = f_position_y(gametime);
    position.z = f_position_z(gametime);
}

glm::mat4 Bullet::model() { return glm::translate(position); }
