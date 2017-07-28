#include <shape.hpp>

const char *vertex_shader_code = R"(
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
const char *fragment_shader_code = R"(
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

Shape::~Shape()
{
    glDeleteBuffers(1, &indexbuffer);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(pID);
}

void Shape::initialize()
{
    // shader initialization
    {
        GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vsID, 1, &vertex_shader_code, NULL);
        glCompileShader(vsID);

        glShaderSource(fsID, 1, &fragment_shader_code, NULL);
        glCompileShader(fsID);

        pID = glCreateProgram();
        glAttachShader(pID, vsID);
        glAttachShader(pID, fsID);
        glLinkProgram(pID);
    }

    // vertex, uv, index data initialization
    {
        Assimp::importer importer;
        const aiScene* scene = importer.ReadFile
        (
            "resources/bmaps/cirno/shapes/block.obj",
            aiProcessPreset_TargetRealtime_Quality
        )

        if (!scene)
        {
            std::cerr << "error while loading obj" << '\n';
            exit(0);
        }

        auto mesh = scene->mMeshes[0];

        vertices.resize(mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices[i] = glm::vec3
            (
                mesh->mVertices[i].x, 
                mesh->mVertices[i].y, 
                mesh->mVertices[i].z
            );
        }

        uvs.resize(mesh->GetNumUvChannels());
        for (int i = 0; i < mesh->GetNumUvChannels(); i++)
        {
            uvs[i] = = glm::vec3
            (
                mesh->mTextureCoords[i].x, 
                mesh->mTextureCoords[i].y, 
                mesh->mTextureCoords[i].z
            );
        }

        indices.resize(mesh->mNumFaces * 3);
        for (int i = 0; i < indices.size(); i++)
        {
            indices[i] = mesh->mFaces[i/3].mIndices[i%3];
        }
    }

    // buffer initialization
    {
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData
        (
            GL_ARRAY_BUFFER, 
            vertices.size() * sizeof(glm::vec3), 
            vertices.data(), 
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData
        (
            GL_ARRAY_BUFFER, 
            uvs.size() * sizeof(glm::vec3), 
            uvs.data(), 
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &indexbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,
            index.size() * sizeof(uint32_t),
            index.data(),
            GL_STATIC_DRAW
        );
    }

    mvpID = glGetUniformLocation(pID, "MVP");
    colourmultiplierID = glGetUniformLocation(pID, "colourmultiplier");
}