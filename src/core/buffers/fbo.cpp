#include <engine/buffers/fbo.hpp>
#include <cassert>
#include <iostream>

FBO::FBO()
{
    msaaFBO = 0;
    msaaColor = 0;
    msaaDepth = 0;

    normalFBO = 0;
    finalColor = 0;
    finalDepth = 0;

    width = 0;
    height = 0;
    multisample = false;
    samples = 1;
}

void FBO::Create(uint32_t w, uint32_t h, bool msaa, int s)
{
    Destroy(); // Safety: recreate cleanly

    width = w;
    height = h;
    multisample = msaa;
    samples = (msaa && s > 1) ? s : 1;

    /* ============================
       MSAA FRAMEBUFFER (OPTIONAL)
       ============================ */
    if (multisample)
    {
        glGenFramebuffers(1, &msaaFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);

        // --- Color buffer ---
        glGenTextures(1, &msaaColor);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaColor);
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE,
            samples,
            GL_RGBA8,
            width,
            height,
            GL_TRUE);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D_MULTISAMPLE,
            msaaColor,
            0);

        // --- Depth-stencil buffer ---
        glGenTextures(1, &msaaDepth);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaDepth);
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE,
            samples,
            GL_DEPTH24_STENCIL8,
            width,
            height,
            GL_TRUE);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D_MULTISAMPLE,
            msaaDepth,
            0);

        // Tell OpenGL we draw into COLOR_ATTACHMENT0
        GLenum msaaAttachments[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, msaaAttachments);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

    /* ============================
       NORMAL FRAMEBUFFER (RESOLVE)
       ============================ */
    glGenFramebuffers(1, &normalFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, normalFBO);

    // --- Color texture ---
    glGenTextures(1, &finalColor);
    glBindTexture(GL_TEXTURE_2D, finalColor);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        finalColor,
        0);

    // --- Depth-stencil texture ---
    glGenTextures(1, &finalDepth);
    glBindTexture(GL_TEXTURE_2D, finalDepth);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH24_STENCIL8,
        width,
        height,
        0,
        GL_DEPTH_STENCIL,
        GL_UNSIGNED_INT_24_8,
        nullptr);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        finalDepth,
        0);

    // Tell OpenGL we draw into COLOR_ATTACHMENT0
    GLenum normalAttachments[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, normalAttachments);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind() const
{
    glBindFramebuffer(
        GL_FRAMEBUFFER,
        multisample ? msaaFBO : normalFBO);

    glViewport(0, 0, width, height);
}

void FBO::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::ResolveMSAA()
{
    if (!multisample)
        return;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, normalFBO);

    glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "FBO incomplete: " << status << std::endl;
    }

    // Restore default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Resize(uint32_t w, uint32_t h)
{
    if (w == width && h == height)
        return;

    Create(w, h, multisample, samples);
}

void FBO::Destroy()
{
    if (msaaColor)
        glDeleteTextures(1, &msaaColor);
    if (msaaDepth)
        glDeleteTextures(1, &msaaDepth);
    if (msaaFBO)
        glDeleteFramebuffers(1, &msaaFBO);

    if (finalColor)
        glDeleteTextures(1, &finalColor);
    if (finalDepth)
        glDeleteTextures(1, &finalDepth);
    if (normalFBO)
        glDeleteFramebuffers(1, &normalFBO);

    msaaColor = 0;
    msaaDepth = 0;
    msaaFBO = 0;

    finalColor = 0;
    finalDepth = 0;
    normalFBO = 0;
}

void FBO::Move(FBO &&other)
{
    Destroy();

    msaaFBO = other.msaaFBO;
    msaaColor = other.msaaColor;
    msaaDepth = other.msaaDepth;

    normalFBO = other.normalFBO;
    finalColor = other.finalColor;
    finalDepth = other.finalDepth;

    width = other.width;
    height = other.height;
    multisample = other.multisample;
    samples = other.samples;

    other.msaaFBO = 0;
    other.msaaColor = 0;
    other.msaaDepth = 0;
    other.normalFBO = 0;
    other.finalColor = 0;
    other.finalDepth = 0;
}
