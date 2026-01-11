#pragma once
#include <memory>
#include <engine/shader.hpp>
// #include <iostream> // Removed for std::cerr

#include <engine/buffers/vao.hpp>
#include <engine/buffers/vbo.hpp>
#include <engine/buffers/ebo.hpp>

#include <engine/utils.h>
#include <engine/texture2D.hpp>

// GL_CHECK_ERROR macro definition (Removed)
// #define GL_CHECK_ERROR() \
//     do { \
//         GLenum err; \
//         while ((err = glGetError()) != GL_NO_ERROR) { \
//             std::cerr << "OpenGL Error: " << err << " in " << __FILE__ << ":" << __LINE__ << std::endl; \
//         } \
//     } while (0)

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 uv;
};
class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<std::shared_ptr<Texture2D>> &textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        for (auto &t : textures)
        {
            this->textures.push_back(std::move(t));
        }
        vao = std::make_unique<VAO>();
        vbo = std::make_unique<VBO<Vertex>>();
        ebo = std::make_unique<EBO>();
        vao->Bind();
        ebo->Bind(indices);
        vbo->Bind(vertices);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(0);


        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, uv)));
        glEnableVertexAttribArray(2);

        vao->Unbind();

        if (this->textures.empty())
        {
            this->textures.push_back(LoadDefaultTexture());
        }
    }

    std::vector<vec3> GetPoints()
    {
        std::vector<vec3> points;
        points.reserve(vertices.size());

        for (const auto &v : vertices)
        {
            points.push_back(v.position);
        }

        return points;
    }

    std::vector<uint32_t> GetIndices()
    {
        return indices;
    }

    void DrawDepth()
    {
        vao->Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        vao->Unbind();
    }

    void Draw(const Shader &shader)
    {
        shader.Use();
    bool diffuseTextureFound = false;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        if (textures[i]->type == Type::DIFFUSE && !diffuseTextureFound)
        {
            textures[i]->Bind(0); // Bind first diffuse texture to unit 0
            shader.SetUniform("diffuse_texture1", 0);
            diffuseTextureFound = true;
        }
        else if (textures[i]->type == Type::SPECULAR)
        {
            // For now, ignoring specular textures as the shader doesn't handle them
            // correctly with multiple units. The fragment shader only has diffuse_texture1.
            // If specular textures are needed, the fragment shader needs to be updated
            // to declare more samplers (e.g., specular_texture1, etc.)
        }
    }

        // draw buffer
        vao->Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        vao->Unbind();
    }

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unique_ptr<VAO> vao = nullptr;
    std::unique_ptr<VBO<Vertex>> vbo = nullptr;
    std::unique_ptr<EBO> ebo = nullptr;
    std::vector<std::shared_ptr<Texture2D>> textures;
    std::shared_ptr<Texture2D> LoadDefaultTexture()
    {
        return std::make_shared<Texture2D>("assets/textures/default_sprite.png");
    }
};