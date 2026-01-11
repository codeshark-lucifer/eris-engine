#pragma once
#include "./bo.hpp"

class EBO
{
public:
    EBO() : bo(GL_ELEMENT_ARRAY_BUFFER) {}

    void Bind(const std::vector<unsigned int> &data,
              GLenum usage = GL_STATIC_DRAW)
    {
        bo.Bind(data, usage);
    }

private:
    BO<unsigned int> bo;
};
