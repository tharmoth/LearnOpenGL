//
// Created by tharmoth on 7/1/2024.
//

#ifndef CUBE_H
#define CUBE_H


const vec3 cube_pos[] = {
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f}
};

const vec3 cube_normal[] = {
    { 0.0f,  0.0f, -1.0f},
    { 0.0f,  0.0f, -1.0f},
    { 0.0f,  0.0f, -1.0f},
    { 0.0f,  0.0f, -1.0f},
    { 0.0f,  0.0f,  1.0f},
    { 0.0f,  0.0f,  1.0f},
    { 0.0f,  0.0f,  1.0f},
    { 0.0f,  0.0f,  1.0f},
    {-1.0f,  0.0f,  0.0f},
    {-1.0f,  0.0f,  0.0f},
    {-1.0f,  0.0f,  0.0f},
    {-1.0f,  0.0f,  0.0f},
    { 1.0f,  0.0f,  0.0f},
    { 1.0f,  0.0f,  0.0f},
    { 1.0f,  0.0f,  0.0f},
    { 1.0f,  0.0f,  0.0f},
    { 0.0f, -1.0f,  0.0f},
    { 0.0f, -1.0f,  0.0f},
    { 0.0f, -1.0f,  0.0f},
    { 0.0f, -1.0f,  0.0f},
    { 0.0f,  1.0f,  0.0f},
    { 0.0f,  1.0f,  0.0f},
    { 0.0f,  1.0f,  0.0f},
    { 0.0f,  1.0f,  0.0f},
};

const vec2 cube_uvs[] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
};

const unsigned int cube_indices[] = {
    // Front face
    2, 1, 0,   0, 3, 2,
    // Back face
    4, 5, 6,   6, 7, 4,
    // Left face
    8, 9, 10,  10, 11, 8,
    // Right face
    14, 13, 12, 12, 15, 14,
    // Bottom face
    16, 17, 18, 18, 19, 16,
    // Top face
    22, 21, 20, 20, 23, 22
};

#endif //CUBE_H