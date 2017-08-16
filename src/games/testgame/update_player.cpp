#include "testgame.hpp"

void TestGame::update_player()
{
    double xpos, ypos;
    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);

    bool go_forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool go_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool go_backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool go_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool go_up = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    bool go_down = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    bool moving = go_forward || go_backward
        || go_left || go_right
        || go_up || go_down;

    auto xdelta = float(width/2 - xpos);
    auto ydelta = float(height/2 - ypos);
    player.head_horizontal_angle += t.delta * xdelta * 0.05f;
    player.head_vertical_angle += t.delta * ydelta * 0.05f;

    player.velocity = glm::vec3(0.0f);
    if (moving)
    {
        player.body_horizontal_angle = player.head_horizontal_angle;
        player.body_vertical_angle = player.head_vertical_angle;
        glm::vec3 direction = glm::vec3
        (
            cos(player.body_vertical_angle) * sin(player.body_horizontal_angle),
            sin(player.body_vertical_angle),
            cos(player.body_vertical_angle) * cos(player.body_horizontal_angle)
        );
        glm::vec3 right = glm::vec3
        (
            sin(player.body_horizontal_angle - 3.14f/2.0f),
            0,
            cos(player.body_horizontal_angle - 3.14f/2.0f)
        );
        glm::vec3 up = glm::cross(right, direction);

        glm::vec3 move =
            (go_forward ? direction : (go_backward ? -direction : glm::vec3(0, 0, 0)))
          + (go_right ? right : (go_left ? -right : glm::vec3(0, 0, 0)))
          + (go_up ? up : (go_down ? -up : glm::vec3(0, 0, 0)));
        move = glm::normalize(move);

        player.velocity = move * t.delta * 10.0f;
    }
}
