#ifndef SHAPE_HPP_
#define SHAPE_HPP_

class Shape
{
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> uvs;
    std::vector<uint32_t> indices;

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint indexbuffer;

    GLuint pID;
    GLuint mvpID;
    GLuint colourmultiplierID;

public:
    ~Shape();

    void initialize();
    void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::mat4 colourmultiplier
    ) const;
};

#endif SHAPE_HPP_
