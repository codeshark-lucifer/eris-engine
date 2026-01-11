#pragma once
#include <glad/glad.h>
#include <cassert>
#include <vector>

class FBO
{
public:
    FBO();
    ~FBO() { Destroy(); }

    // Non-copyable
    FBO(const FBO&) = delete;
    FBO& operator=(const FBO&) = delete;

    // Movable
    FBO(FBO&& other) noexcept { Move(std::move(other)); }
    FBO& operator=(FBO&& other) noexcept
    {
        Destroy();
        Move(std::move(other));
        return *this;
    }

    // Create FBO, optionally MSAA
    void Create(uint32_t width, uint32_t height, bool msaa = false, int samples = 4);

    void Resize(uint32_t width, uint32_t height);

    // Bind for rendering
    void Bind() const;
    static void Unbind();

    // If MSAA, resolve internally
    void ResolveMSAA();

    // Always returns the usable texture (resolved or normal)
    GLuint GetColorTexture() const { return finalColor; }

    GLuint GetDepthTexture() const { return finalDepth; }

private:
    GLuint msaaFBO = 0;
    GLuint msaaColor = 0;
    GLuint msaaDepth = 0;

    GLuint normalFBO = 0;
    GLuint finalColor = 0;
    GLuint finalDepth = 0;

    uint32_t width = 0;
    uint32_t height = 0;
    bool multisample = false;
    int samples = 1;

    void CreateAttachments();
    void Destroy();
    void Move(FBO&& other);
};
