//
// Created by Tharmoth on 6/22/2024.
//

#include <glad/glad.h>
#include "mesh.h"

Vertex mesh_new(const float *vertices, const int verticesSize)
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // // texture uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Vertex vertex;
    vertex.ID = VAO;
    return vertex;
}

Vertex mesh_new_element(const float *vertices, const int verticesSize, const int *indices, const int indicesSize)
{
    Vertex vertex = mesh_new(vertices, verticesSize);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    vertex.EBO = EBO;

    return vertex;
}

void mesh_draw(const Vertex vertex)
{
    glBindVertexArray(vertex.ID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void mesh_draw_element(const Vertex vertex) {
    glBindVertexArray(vertex.ID);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex.EBO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}