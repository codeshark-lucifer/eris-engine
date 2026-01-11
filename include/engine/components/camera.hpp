#pragma once
#include <engine/ecs/entity.hpp>
#include <engine/shader.hpp>
#include <iostream>

class Camera : public Component
{
public:
    ~Camera() = default;

    void Initialize(const int &width, const int &height)
    {
        this->width = width;
        this->height = height;
    }

    void Resize(int w, int h)
    {
        if (this->width == w && this->height == h)
            return;
        this->width = w;
        this->height = h;
    }

    mat4 GetProjection() const
    {
        return mat4::Perspective(mathf::radians(fov), (float)width / (float)height, near, far);
    }

    mat4 GetView() const
    {
        vec3 position = entity->transform.position;
        vec3 forward = entity->transform.rotation * vec3(0, 0, -1);
        vec3 up = entity->transform.rotation * vec3(0, 1, 0);

        return mat4::LookAt(position, position + forward, up);
    }

    void SetUniform(const Shader &shader)
    {
        if (!entity)
            return;
        mat4 projection = GetProjection();
        mat4 view = GetView();
        
        shader.Use();
        shader.SetUniform("projection", projection);
        shader.SetUniform("view", view);
        shader.SetUniform("viewPos", entity->transform.position);
    }

private:
    int width = 0, height = 0;
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = vec3(0.0f);

    vec3 target = vec3(0.0f);
    vec3 direction = vec3(0.0f);

    float fov = 60.0f;
    float near = 0.3f;
    float far = 300.0f;
};