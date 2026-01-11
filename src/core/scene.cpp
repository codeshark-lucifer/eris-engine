#include <engine/scene.hpp>
#include <engine/model.hpp>
#include <iostream>

bool Scene::Initialize(const int &width, const int &height, const char *name)
{
    render = std::make_unique<RenderSystem>();
    update = std::make_unique<UpdateSystem>();

    render->Initialize(width, height);
    return true;
}

void Scene::Start()
{
    render->Start(entities);
    update->Start(entities);
}

void Scene::Update(const float &deltaTime)
{
    // process update, render here
    update->Update(entities, deltaTime);

    for (const auto &[id, entity] : entities)
    {
        ComputeEntitiesMatrix(id);
    }

    render->Render(entities);
}

void Scene::Resize(const int &width, const int &height)
{
    render->Resize(entities, width, height);
}

Entity *Scene::FindEntityByID(EntityID id) const
{
    auto it = entities.find(id);
    if (it != entities.end())
        return it->second.get();
    return nullptr;
}

Entity *Scene::CreateEntity(const char *name)
{
    std::cout << "*Create Entity: " << name << std::endl;
    auto entity = std::make_unique<Entity>(name);
    EntityID id = entity->GetID();

    auto [it, inserted] = entities.emplace(id, std::move(entity));
    return it->second.get();
}

Entity *Scene::LoadModel(const char *path)
{
    return Model::Load(path, this);
}

void Scene::ComputeEntitiesMatrix(EntityID id) const
{
    Entity *current = FindEntityByID(id);
    if (!current)
        return;

    mat4 result = mat4::Identity();

    // Walk up the hierarchy
    while (current)
    {
        result = current->transform.local() * result;

        if (current->transform.parent == INVALID_ENTITY_ID)
            break;

        current = FindEntityByID(current->transform.parent);
    }

    // Store final world matrix
    Entity *target = FindEntityByID(id);
    if (target)
        target->transform.matrix = result;
}
