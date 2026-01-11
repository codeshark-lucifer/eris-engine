#pragma once
#include "vector.h"

struct Properties
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    static const Properties RED;
    static const Properties GREEN;
    static const Properties BLUE;
    static const Properties WHITE;
};
