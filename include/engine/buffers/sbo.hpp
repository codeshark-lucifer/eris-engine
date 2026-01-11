#pragma once
#include <engine/components/light.hpp>
struct ShadowMatrices
{
    mat4 matrices[6];
    int count = 1; // 1 for dir/spot, 6 for point
};

class SBO
{
public:
    SBO();
    ~SBO();
    void Destroy();
    void Create(int size);
    void Bind();
    void Unbind();
    ShadowMatrices ComputeMatrices(const Light* light) const;
    unsigned int GetShaodwMapTexture() { return shadowMap; }

private:
    int size = 0;
    unsigned int ID = 0;
    unsigned int shadowMap = 0;
};