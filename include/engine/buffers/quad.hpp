#pragma once
#include <glad/glad.h>
#include <memory>
#include <engine/shader.hpp>

class Quad
{
public:
    Quad();
    ~Quad();

    // Non-copyable
    Quad(const Quad &) = delete;
    Quad &operator=(const Quad &) = delete;

    // Movable
    Quad(Quad &&) noexcept = default;
    Quad &operator=(Quad &&) noexcept = default;

    // Draw the quad
    void Draw() const;

private:
    GLuint VAO = 0, VBO = 0;
};
