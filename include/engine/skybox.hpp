#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
#include <stb/stb_image.h>
#include <engine/shader.hpp>
#include <engine/buffers/vao.hpp>
#include <engine/buffers/vbo.hpp>
#include <engine/buffers/ebo.hpp>

#include <engine/components/camera.hpp>

class Skybox
{
public:
    Skybox()
    {
        shader = new Shader("assets/shaders/skybox.glsl");
    }

    ~Skybox()
    {
        if (shader) delete shader;
    }

    void Create(const std::string &path)
    {
        std::vector<std::string> faces = {
            path + "/right.jpg",
            path + "/left.jpg",
            path + "/top.jpg",
            path + "/bottom.jpg",
            path + "/back.jpg",
            path + "/front.jpg"
        };

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cerr << "Failed to load skybox texture: " << faces[i] << std::endl;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Cube vertices (positions only)
        vertices = {
            {-1.0f,  1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f},
            {-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f},
            {1.0f, -1.0f, -1.0f}, {1.0f, -1.0f,  1.0f}, {1.0f,  1.0f,  1.0f}, {1.0f,  1.0f,  1.0f}, {1.0f,  1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, {1.0f,  1.0f,  1.0f}, {1.0f,  1.0f,  1.0f}, {1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f},
            {-1.0f,  1.0f, -1.0f}, {1.0f,  1.0f, -1.0f}, {1.0f,  1.0f,  1.0f}, {1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f}, {1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f}, {1.0f, -1.0f,  1.0f}
        };

        vao.Bind();
        vbo.Bind(vertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
        glEnableVertexAttribArray(0);
        vao.Unbind();
    }

    void Draw(Camera* camera)
    {
        glDepthFunc(GL_LEQUAL); // Skybox needs less/equal depth
        shader->Use();

        // Remove translation from view
        mat4 view = mat4(mat3(camera->GetView()));
        mat4 projection = camera->GetProjection();

        shader->SetUniform("view", view);
        shader->SetUniform("projection", projection);

        vao.Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        shader->SetUniform("skybox", 0); // match shader uniform
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        vao.Unbind();
        glDepthFunc(GL_LESS);
    }

private:
    unsigned int ID = 0;
    Shader* shader = nullptr;
    VAO vao;
    VBO<vec3> vbo;
    std::vector<vec3> vertices;
};
