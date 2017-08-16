#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "colliders/boxcollider.hpp"

struct Player
{
    glm::vec3 position;
    glm::vec3 velocity;
    float body_horizontal_angle;
    float body_vertical_angle;
    float head_horizontal_angle;
    float head_vertical_angle;
    BoxCollider box;

    Player()
        : position(0, 0, 0)
        , velocity(0, 0, 0)
        , body_horizontal_angle(0)
        , body_vertical_angle(0)
        , head_horizontal_angle(0)
        , head_vertical_angle(0)
        , box(glm::vec3(0.5f))
    { }
};

#endif /* end of include guard: PLAYER_HPP */
