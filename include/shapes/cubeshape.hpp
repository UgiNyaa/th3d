#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <string>

#include <shapes/shape.hpp>
#include <colliders/boxcollider.hpp>
#include <drawers/cubedrawer.hpp>

struct CubeShape : public Shape
{
    CubeDrawer cube;
    BoxCollider box;

    CubeShape()
        : box(glm::vec3(1.0f))
    { }

    virtual ~CubeShape() { };

    std::string name() const override { return "Cube"; }
    Collider const& collider() const override { return box; }
    void initialize() override { cube.initialize(); }
    void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    ) const { cube.draw(model, view, projection, colourmultiplier); }
};

#endif /* end of include guard: CUBE_HPP */
