#pragma once
#include <engine/ecs/entity.hpp>
#include <engine/mesh.hpp>

class MeshFilter : public Component
{
public:
    std::unique_ptr<Mesh> mesh = nullptr;
};
