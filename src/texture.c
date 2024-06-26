//
// Created by Tharmoth on 6/22/2024.
//

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stdbool.h>

#include "stb_image.h"

Texture texture_new(const char* texture_file_path, const GLenum format)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texture_file_path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ: %s\n", texture_file_path);
    }
    stbi_image_free(data);

    Texture t;
    t.ID = texture;
    return t;
}

void texture_bind(const Texture* texture, const int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture->ID);
}


