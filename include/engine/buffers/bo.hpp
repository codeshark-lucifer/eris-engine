#pragma once
#include <glad/glad.h>
#include <vector>

template <typename T>
class BO
{
public:
    BO(GLenum n = GL_ARRAY_BUFFER) : target(n)
    {
        glGenBuffers(1, &ID);
    }

    ~BO()
    {
        if(ID != 0) glDeleteBuffers(1, &ID);
    }

    void Bind(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
        glBindBuffer(target, ID);
        glBufferData(target, data.size() * sizeof(T), data.data(), usage);
    }

private:
    unsigned int ID = 0;
    GLenum target;
};