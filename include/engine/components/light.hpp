#pragma once
#include <engine/ecs/entity.hpp>

enum class LightType : uint32_t
{
    Directional = 0,
    Point = 1,
    Spot = 2
};
/*
Light:
- direction : xyz ( vec3 )
- color : rgb + intensity
- prams : range, inner, outer, castsShadow ( int )
*/
class Light : public Component
{
public:
    LightType type = LightType::Directional;

    vec3 direction = vec3(-0.2f, -1.0f, -0.3f); // for directional & spot
    vec4 color = vec4(vec3(1.0f), 1.0f);        // RGB + intensity

    // parameters
    float range = 10.0f;       // for point/spot
    float innerCutOff = 15.0f; // degrees
    float outerCutOff = 17.0f; // degrees
    bool castsShadow = true;   // for future shadows

    // attenuation (optional, can calculate automatically)
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};
