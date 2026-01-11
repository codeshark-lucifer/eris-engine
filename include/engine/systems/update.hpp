#include <engine/ecs/system.hpp>

class UpdateSystem : public System
{
public:
    UpdateSystem() : System("UpdateSystem") {}
    void Start(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities) override
    {
        for (const auto &[id, entity] : entities)
        {
            for (const auto &component : entity->GetAllComponent())
            {
                component->Start();
            }
        }
    }

    void Update(const std::unordered_map<EntityID, std::unique_ptr<Entity>> &entities, const float &deltaTime) override
    {
        for (const auto &[id, entity] : entities)
        {
            for (const auto &component : entity->GetAllComponent())
            {
                component->Update(deltaTime);
            }
        }
    }
};