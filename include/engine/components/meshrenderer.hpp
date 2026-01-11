#pragma once
#include <engine/components/meshfilter.hpp>
#include <engine/shader.hpp>
#include <iostream>

class MeshRenderer : public Component
{
public:
    void Draw(const Shader &shader, bool isDepth = false)
    {
        if (!entity)
            return;
        SetUniform(shader);
        auto meshfilter = entity->GetComponent<MeshFilter>();
        if (!meshfilter || !meshfilter->mesh)
            return;

        if (isDepth)
            meshfilter->mesh->DrawDepth();
        else
            meshfilter->mesh->Draw(shader);
    }

private:
    void SetUniform(const Shader &shader)
    {
        shader.Use();
        shader.SetUniform("model", entity->transform.matrix);
    }
};