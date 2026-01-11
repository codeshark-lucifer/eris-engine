#pragma once
#include "./bo.hpp"

template <typename T>
class VBO
{
public:
    VBO() : bo(GL_ARRAY_BUFFER) {}

    void Bind(const std::vector<T> &data, GLenum usage = GL_STATIC_DRAW)
    {
        bo.Bind(data, usage);
    }

private:
    BO<T> bo;
};