//
// Created by Tharmoth on 6/22/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

typedef struct
{
    unsigned int ID;
} Texture;

Texture texture_new(const char* texture_file_path, const GLenum format);

#endif //TEXTURE_H
