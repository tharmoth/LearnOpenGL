//
// Created by Tharmoth on 6/22/2024.
//

#include <glad/glad.h>
#include "mesh.h"

Mesh mesh_process(
    vec3* position,
    vec3* normal,
    vec2* uv,
    const unsigned int vertexCount,
    const int* indices,
    const unsigned int indexCount) {
    Vertex vertices[vertexCount];
    // Create the vertices to feed into the mesh
    for(int i = 0; i < vertexCount; i++)
    {
        Vertex vertex;
        glm_vec3_copy(position[i], vertex.position);
        glm_vec3_copy(normal[i], vertex.normal);
        glm_vec2_copy(uv[i], vertex.uv);
        vertices[i] = vertex;
    }

    Texture* textures = malloc(sizeof(Texture));
    textures[0] = texture_new("data/dirt.png", GL_RGBA);
    return mesh_new(vertices, sizeof(vertices), indices, sizeof(indices) * indexCount, textures, 1);
}

Mesh mesh_new(
    const Vertex *vertices,
    const int verticesSize,
    const int *indices,
    const int indicesSize,
    Texture* textures,
    int texturesSize
    ) {

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // // texture uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    const Mesh mesh = {
        .VAO = VAO,
        .textures = textures,
        .texturesSize = texturesSize
    };
    return mesh;
}

void mesh_draw(const Mesh* mesh, const Shader shader) {
    texture_bind(&mesh->textures[0], 0);
    shader_set_int(shader, "material.diffuse", 0);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void mesh_free(Mesh* mesh) {
    free(mesh->textures);
}