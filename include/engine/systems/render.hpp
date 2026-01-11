#pragma once
#include <engine/ecs/system.hpp>
#include <glad/glad.h>

#include <engine/components/meshrenderer.hpp>
#include <engine/components/camera.hpp>
#include <engine/components/light.hpp>
#include <engine/components/material.hpp>

#include <engine/buffers/fbo.hpp>
#include <engine/buffers/sbo.hpp>
#include <engine/buffers/quad.hpp>

#include <iostream>
#include <cassert>

class RenderSystem : public System
{
public:
    RenderSystem() : System("RenderSystem") {}

    void Initialize(int w, int h)
    {
        width = w;
        height = h;
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);

        defaultShader = std::make_unique<Shader>("assets/shaders/scene.vert", "assets/shaders/scene.frag");
        depthShader = std::make_unique<Shader>("assets/shaders/depth.glsl");
        bufferShader = std::make_unique<Shader>("assets/shaders/screen.glsl");

        framebuffer.Create(width, height, true, 8);
    }

    void Start(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities) override
    {
        ResetCamera(entities);

        // Collect lights and create shadow buffers
        for (const auto &[id, entity] : entities)
        {
            for (const auto &comp : entity->GetAllComponent())
            {
                if (auto light = dynamic_cast<Light *>(comp))
                    AddLight(light);
            }
        }
    }

    void ResetCamera(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities)
    {
        for (const auto &[id, entity] : entities)
        {
            for (const auto &comp : entity->GetAllComponent())
            {
                if (auto camera = dynamic_cast<Camera *>(comp))
                {
                    defaultCamera = camera;
                    defaultCamera->Initialize(width, height);
                    return;
                }
            }
        }
    }

    void Render(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities) override
    {
        if (!defaultCamera)
            ResetCamera(entities);
        if (!defaultCamera)
        {
            std::cerr << "No camera found!" << std::endl;
            return;
        }
        mat4 lightVP;
        for (const auto &[sbo, light] : lights)
        {
            lightVP = sbo->ComputeMatrices(light).matrices[0];
            sbo->Bind();
            depthShader->Use();
            depthShader->SetUniform("lightViewProjection", lightVP);
            sbo->Unbind();
            glViewport(0, 0, width, height);
        }

        framebuffer.Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        defaultShader->Use();
        defaultCamera->SetUniform(*defaultShader);
        int lightCount = 0;
        for (const auto &[sbo, light] : lights)
        {
            std::string name = "lights[" + std::to_string(lightCount) + "].";
            defaultShader->SetUniform(name + "color", light->color.xyz() * light->color.w);

            defaultShader->SetUniform(name + "type", static_cast<int>(light->type));
            if (light->type == LightType::Directional)
            {
                defaultShader->SetUniform(name + "direction", light->direction.Normalized());
            }
            else if (light->type == LightType::Point)
            {
                defaultShader->SetUniform(name + "position", light->entity->transform.position);
                defaultShader->SetUniform(name + "constant", light->constant);
                defaultShader->SetUniform(name + "linear", light->linear);
                defaultShader->SetUniform(name + "quadratic", light->quadratic);
            }
            else if (light->type == LightType::Spot)
            {
                defaultShader->SetUniform(name + "direction", light->direction.Normalized());
                defaultShader->SetUniform(name + "position", light->entity->transform.position);
                defaultShader->SetUniform(name + "constant", light->constant);
                defaultShader->SetUniform(name + "linear", light->linear);
                defaultShader->SetUniform(name + "quadratic", light->quadratic);
                defaultShader->SetUniform(name + "cutOff", cos(mathf::radians(light->innerCutOff)));
                defaultShader->SetUniform(name + "outerCutOff", cos(mathf::radians(light->outerCutOff)));
            }

            lightVP = sbo->ComputeMatrices(light).matrices[0];
            defaultShader->SetUniform("lightViewProjection[" + std::to_string(lightCount) + "]", lightVP);

            int unit = 2 + lightCount;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, sbo->GetShaodwMapTexture());
            defaultShader->SetUniform("shadowMaps[" + std::to_string(lightCount) + "]", unit);

            lightCount++;
        }
        defaultShader->SetUniform("lightCount", lightCount);

        Draw(entities, false);
        framebuffer.Unbind();
        framebuffer.ResolveMSAA();
        glDisable(GL_DEPTH_TEST);

        bufferShader->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, framebuffer.GetColorTexture());
        bufferShader->SetUniform("screenTexture", 0);
        screen.Draw();
    }

    void Resize(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities, int w, int h) override
    {
        width = w;
        height = h;
        if (defaultCamera)
            defaultCamera->Resize(width, height);
        glViewport(0, 0, width, height);
        framebuffer.Resize(width, height);
    }

private:
    int width = 0, height = 0;
    std::unique_ptr<Shader> defaultShader;
    std::unique_ptr<Shader> depthShader;
    std::unique_ptr<Shader> bufferShader;
    std::vector<std::pair<std::unique_ptr<SBO>, Light *>> lights;
    Camera *defaultCamera = nullptr;
    FBO framebuffer;
    Quad screen;

    void AddLight(Light *light)
    {
        assert(light && "Light pointer is null");
        auto sbo = std::make_unique<SBO>();
        sbo->Create(2048); // Shadow map resolution
        lights.emplace_back(std::move(sbo), light);
    }

    void Draw(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities, bool isDepth)
    {
        for (const auto &[id, entity] : entities)
        {
            Material *material = nullptr;
            MeshRenderer *renderer = nullptr;

            for (const auto &comp : entity->GetAllComponent())
            {
                if (!material)
                    material = dynamic_cast<Material *>(comp);
                if (!renderer)
                    renderer = dynamic_cast<MeshRenderer *>(comp);
            }

            if (!renderer)
                continue;

            Properties mat;
            if (material)
                mat = material->properties;
            else
            {
                mat.ambient = vec3(1, 0, 1);
                mat.diffuse = vec3(1, 0, 1);
                mat.specular = vec3(1);
                mat.shininess = 32.0f;
            }

            if (!isDepth)
            {
                defaultShader->SetUniform("material.ambient", mat.ambient);
                defaultShader->SetUniform("material.diffuse", mat.diffuse);
                defaultShader->SetUniform("material.specular", mat.specular);
                defaultShader->SetUniform("material.shininess", mat.shininess);
            }

            renderer->Draw(isDepth ? *depthShader : *defaultShader, isDepth);
        }
    }
};