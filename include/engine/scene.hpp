#pragma once
#include <engine/ecs/entity.hpp>
#include <engine/systems/render.hpp>
#include <engine/systems/update.hpp>

class Scene
{
public:
    bool Initialize(const int &width, const int &height, const char *name);

    void Start();
    void Update(const float &deltaTime);
    void Resize(const int &width, const int &height);
    Entity *FindEntityByID(EntityID id) const;
    Entity *CreateEntity(const char *name);
    Entity *LoadModel(const char *path);

private:
    std::unique_ptr<RenderSystem> render;
    std::unique_ptr<UpdateSystem> update;
    void ComputeEntitiesMatrix(EntityID id) const;
    std::unordered_map<EntityID, std::unique_ptr<Entity>> entities;
};
