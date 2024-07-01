//
// Created by Tharmoth on 6/22/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

typedef struct
{
    unsigned int ID;
    const char* type;
} Texture;

Texture texture_new(const char* texture_file_path, const GLenum format);
void texture_bind(const Texture* texture, const int index);

#endif //TEXTURE_H
