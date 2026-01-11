#pragma once
#include "./component.hpp"
#include <engine/utils.h>
#include <unordered_map>
#include <vector>
#include <atomic>
#include <memory>
#include <typeindex>
#include <typeinfo>

using EntityID = uint32_t;
constexpr EntityID INVALID_ENTITY_ID = 0;

struct Transform
{
    vec3 position{};
    quat rotation{};
    vec3 scale{1.0f};

    EntityID parent = INVALID_ENTITY_ID;
    std::vector<EntityID> children;

    mat4 matrix = mat4::Identity();

    mat4 local() const
    {
        return mat4::Translate(position) * mat4::Rotate(rotation) * mat4::Scale(scale);
    }
};

class Entity
{
public:
    const char *name = "New Entity";
    Transform transform;

public:
    Entity(const char *name) : name(name), id(GenID()) {}
    EntityID GetID() const { return id; }

    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->entity = this;
        T *ptr = comp.get();
        components[std::type_index(typeid(T))].push_back(std::move(comp));
        return ptr;
    }

    template <typename T>
    std::vector<T *> GetComponents()
    {
        std::vector<T *> result;
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end())
        {
            for (auto &compPtr : it->second)
                result.push_back(static_cast<T *>(compPtr.get()));
        }
        return result;
    }

    template <typename T>
    T *GetComponent()
    {
        auto comps = GetComponents<T>();
        return comps[0];
    }

    std::vector<Component *> GetAllComponent() const
    {
        std::vector<Component *> result;
        for (const auto &[idx, pairs] : components)
        {
            for (const auto &comp : pairs)
            {
                result.push_back(comp.get());
            }
        }
        return result;
    }

private:
    std::unordered_map<std::type_index, std::vector<std::unique_ptr<Component>>> components;
    EntityID id;

    static EntityID GenID()
    {
        static std::atomic<EntityID> counter{1};
        return counter++;
    }
};