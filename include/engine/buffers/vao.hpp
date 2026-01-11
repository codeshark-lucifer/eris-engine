#pragma once
#include <glad/glad.h>

class VAO
{
public:
    VAO()
    {
        glGenVertexArrays(1, &ID);
    }

    ~VAO()
    {
        if (ID != 0)
            glDeleteVertexArrays(1, &ID);
    }

    void Bind()
    {
        glBindVertexArray(ID);
    }

    void Unbind()
    {
        glBindVertexArray(0);
    }

private:
    unsigned int ID = 0;
};