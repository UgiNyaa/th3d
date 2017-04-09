#include <games/testgame/testgame.hpp>

glm::mat4 TestGame::player_model()
{
    return glm::translate(player.position) *
        glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
}

glm::mat4 TestGame::camera_view()
{
    glm::vec3 direction = glm::vec3(
    cos(player.head_vertical_angle) *
        sin(player.head_horizontal_angle),

    sin(player.head_vertical_angle),

    cos(player.head_vertical_angle) *
        cos(player.head_horizontal_angle)
    );
    glm::vec3 right = glm::vec3(
        sin(player.head_horizontal_angle - 3.14f/2.0f),
        0,
        cos(player.head_horizontal_angle - 3.14f/2.0f)
    );
    glm::vec3 up = glm::cross(right, direction);

    return glm::lookAt
    (
        player.position
      - (direction
      * camera.distance),
        player.position
      + direction,
        up
    );
}

glm::mat4 TestGame::camera_projection()
{
    return glm::perspective(
        glm::radians(camera.field_of_view),
        1280.0f / 720.0f,
        0.1f, 100.0f
    );
}
