#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <shapes/cube.hpp>

// How buffer looks like:
// A = wanted value
// B = unwanted value
// (F) = type is float
// - = use a new variable

/*void check_shader(GLuint shaderID)
{
    GLint Result = GL_FALSE;
    int32_t InfoLogLength;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        printf("%s\n", &ShaderErrorMessage[0]);
    }
}*/

/*
How to define a quad is defined here:
0,1       1,1
+---------+
|        /|
|      /  |
|    /    |
|  /      |
|/        |
+---------+
0,0       1,0
Always start with lower left point
Go counter clockwise and define the first triangle
    => lower tirangle
Continue with the upper right point
Go counter clockwise and define the second triangle
    => upper triangle
!lower tri end point = upper tri start point!
!upper tri end point = lower tri start point!
!0,0 and 1,1 are defined twice!
*/
const GLfloat Cube::g_vertex_buffer_data[] = {
    // front-lower triangle
    -1.0f,-1.0f, /**/1.0f,
    +1.0f,-1.0f, /**/1.0f,
    +1.0f,+1.0f, /**/1.0f,
    // front-upper triangle
    +1.0f,+1.0f, /**/1.0f,
    -1.0f,+1.0f, /**/1.0f,
    -1.0f,-1.0f, /**/1.0f,
    // front-lower triangle
    -1.0f,-1.0f, /**/-1.0f,
    +1.0f,-1.0f, /**/-1.0f,
    +1.0f,+1.0f, /**/-1.0f,
    // front-upper triangle
    +1.0f,+1.0f, /**/-1.0f,
    -1.0f,+1.0f, /**/-1.0f,
    -1.0f,-1.0f, /**/-1.0f,
    // left-lower triangle
    /**/-1.0f, -1.0f,-1.0f,
    /**/-1.0f, +1.0f,-1.0f,
    /**/-1.0f, +1.0f,+1.0f,
    // left-upper triangle
    /**/-1.0f, +1.0f,+1.0f,
    /**/-1.0f, -1.0f,+1.0f,
    /**/-1.0f, -1.0f,-1.0f,
    // right-lower triangle
    /**/1.0f, -1.0f,-1.0f,
    /**/1.0f, +1.0f,-1.0f,
    /**/1.0f, +1.0f,+1.0f,
    // right-upper triangle
    /**/1.0f, +1.0f,+1.0f,
    /**/1.0f, -1.0f,+1.0f,
    /**/1.0f, -1.0f,-1.0f,
};
const GLfloat Cube::g_uv_buffer_data[] = {
    // front-lower triangle
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    // front-upper triangle
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    // back-lower triangle
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    // back-upper triangle
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    // left-lower triangle
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    // left-upper triangle
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    // right-lower triangle
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    // right-upper triangle
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};
const char *Cube::vertex_shader_code = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

    UV = vertexUV;
})";
const char *Cube::fragment_shader_code = R"(
#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D myTextureSampler;
uniform vec3 colourmultiplier;

void main()
{
    color.rgb = texture(myTextureSampler, UV).rgb;
    color.rgb = color.rgb * colourmultiplier;
}
)";

Cube::Cube()
    : box(glm::vec3(1.0f))
{
    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vsID, 1, &vertex_shader_code, NULL);
    glCompileShader(vsID);
    //check_shader(vsID);
    glShaderSource(fsID, 1, &fragment_shader_code, NULL);
    glCompileShader(fsID);
    //check_shader(fsID);

    pID = glCreateProgram();
    glAttachShader(pID, vsID);
    glAttachShader(pID, fsID);
    glLinkProgram(pID);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    mvpID = glGetUniformLocation(pID, "MVP");
    colourmultiplierID = glGetUniformLocation(pID, "colourmultiplier");
}

Cube::~Cube()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(pID);
}

void Cube::draw
(
    glm::mat4 model,
    glm::mat4 view,
    glm::mat4 projection,
    glm::vec3 colourmultiplier
)
{
    glUseProgram(pID);

    glm::mat4 mvp = projection * view * model;

    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
    glUniform3fv(colourmultiplierID, 1, &colourmultiplier[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // A(F)A(F)A(F)-A(F)A(F)A(F)-A(F)A(F)A(F)...
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    // A(F)A(F)A(F)-A(F)A(F)A(F)-A(F)A(F)A(F)...
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3*8);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
