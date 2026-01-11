#pragma once
#include <engine/ecs/entity.hpp>
#include <engine/utils/color.h>

class Material : public Component
{
public:
    Properties properties = Properties::WHITE;
};