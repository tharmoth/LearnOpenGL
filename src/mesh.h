//
// Created by Tharmoth on 6/22/2024.
//

#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include "cglm/cglm.h"
#include "texture.h"

typedef struct
{
    unsigned int VAO;
    const Texture* textures;
    const unsigned int texturesSize;
} Mesh;

typedef struct
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} Vertex;

Mesh mesh_process(vec3* position,
    vec3* normal,
    vec2* uv,
    const unsigned int vertexCount,
    const int* indices,
    const unsigned int indexCount);

Mesh mesh_new(
    const Vertex *vertices,
    const int verticesSize,
    const int *indices,
    const int indicesSize,
    Texture* textures,
    int texturesSize
    );

void mesh_draw(const Mesh* mesh, const Shader shader);

#endif MESH_H
