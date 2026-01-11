#pragma once
#include <engine/ecs/entity.hpp>

class Rotator : public Component
{
public:
    // Rotation speed in degrees per second (X, Y, Z)
    vec3 speed{ 10.0f, 20.0f, 30.0f };

    Rotator() = default;
    Rotator(const vec3& s) : speed(s) {}

    void Update(const float& deltaTime) override
    {
        Transform& transform = entity->transform;

        // Convert degrees/sec → radians/frame
        vec3 radians = speed * (deltaTime * 0.01745329251f); // DEG2RAD

        // Build incremental rotation quaternion (XYZ order)
        quat qx = quat::FromAxisAngle({1, 0, 0}, radians.x);
        quat qy = quat::FromAxisAngle({0, 1, 0}, radians.y);
        quat qz = quat::FromAxisAngle({0, 0, 1}, radians.z);

        quat deltaRot = qz * qy * qx;

        // Apply rotation (local space)
        transform.rotation = (deltaRot * transform.rotation).Normalized();
    }
};
