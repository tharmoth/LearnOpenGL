//
// Created by Tharmoth on 6/22/2024.
//

#ifndef VERTEX_H
#define VERTEX_H

typedef struct
{
    unsigned int ID;
} Vertex;

Vertex vertex_new(const float *vertices, int verticesSize);
Vertex vertex_new_element(const float *vertices, const int verticesSize, const int *indices, const int indicesSize);
void vertex_draw(Vertex vertex);

#endif //VERTEX_H
