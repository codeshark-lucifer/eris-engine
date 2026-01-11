#include <engine/buffers/sbo.hpp>
#include <glad/glad.h>

SBO::SBO()
{
    ID = 0;
    shadowMap = 0;
}

SBO::~SBO()
{
    Destroy();
}

void SBO::Destroy()
{
    if (ID)
        glDeleteFramebuffers(1, &ID);

    if (shadowMap)
        glDeleteTextures(1, &shadowMap);
}

void SBO::Create(int size = 2048)
{
    this->size = size;
    glGenFramebuffers(1, &ID);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_DEPTH_COMPONENT,
        size, size, 0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float clampColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SBO::Bind()
{
    glViewport(0, 0, size, size);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void SBO::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMatrices SBO::ComputeMatrices(const Light* light) const
{
    ShadowMatrices result{};
    result.count = 1;

    if (light->type == LightType::Directional)
    {
        mat4 projection = mat4::Ortho(
            -35.0f, 35.0f,
            -35.0f, 35.0f,
            0.1f, light->range * 20.0f);

        vec3 lightDir = light->direction.Normalized();

        mat4 view = mat4::LookAt(
            -lightDir * 40.0f,
            vec3(0.0f),
            vec3(0.0f, 1.0f, 0.0f));

        result.matrices[0] = projection * view;
    }
    else if (light->type == LightType::Spot)
    {
        mat4 projection = mat4::Perspective(
            mathf::radians(light->outerCutOff * 2.0f),
            1.0f,
            0.1f,
            light->range);

        vec3 pos = light->entity->transform.position;
        vec3 dir = light->direction.Normalized();

        mat4 view = mat4::LookAt(
            pos,
            pos + dir,
            vec3(0.0f, 1.0f, 0.0f));

        result.matrices[0] = projection * view;
    }
    else if (light->type == LightType::Point)
    {
        result.count = 6;

        float nearPlane = 0.1f;
        float farPlane  = light->range;

        mat4 projection = mat4::Perspective(
            mathf::radians(90.0f),
            1.0f,
            nearPlane,
            farPlane);

        vec3 pos = light->entity->transform.position;

        result.matrices[0] = projection * mat4::LookAt(pos, pos + vec3( 1,  0,  0), vec3(0, -1,  0));
        result.matrices[1] = projection * mat4::LookAt(pos, pos + vec3(-1,  0,  0), vec3(0, -1,  0));
        result.matrices[2] = projection * mat4::LookAt(pos, pos + vec3( 0,  1,  0), vec3(0,  0,  1));
        result.matrices[3] = projection * mat4::LookAt(pos, pos + vec3( 0, -1,  0), vec3(0,  0, -1));
        result.matrices[4] = projection * mat4::LookAt(pos, pos + vec3( 0,  0,  1), vec3(0, -1,  0));
        result.matrices[5] = projection * mat4::LookAt(pos, pos + vec3( 0,  0, -1), vec3(0, -1,  0));
    }

    return result;
}
