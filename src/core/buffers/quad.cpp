#include <engine/buffers/quad.hpp>

// Vertex data for a fullscreen quad
static float quadVertices[] = {
    // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f};

Quad::Quad()
{
    // Setup VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    // texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Quad::~Quad()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Quad::Draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
