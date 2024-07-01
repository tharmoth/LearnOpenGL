//
// Created by Tharmoth on 6/22/2024.
//

#ifndef MESH_H
#define MESH_H

typedef struct
{
    unsigned int ID;
    unsigned int EBO;
} Vertex;

Vertex mesh_new(const float *vertices, int verticesSize);
Vertex mesh_new_element(const float *vertices, const int verticesSize, const int *indices, const int indicesSize);
void mesh_draw(Vertex vertex);
void mesh_draw_element(Vertex vertex);

#endif MESH_H
