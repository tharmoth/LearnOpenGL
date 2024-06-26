//
// Created by Tharmoth on 6/22/2024.
//

#ifndef SHADER_H
#define SHADER_H

typedef struct
{
    unsigned int ID;
} Shader;

Shader shader_new(const char* vertex_file_path, const char* fragment_file_path);
void shader_use(const Shader shader);
void shader_set_int(const Shader shader, const char* name, const int value);
void shader_set_bool(const Shader shader, const char* name, const int value);
void shader_set_float(const Shader shader, const char* name, const float value);
void shader_set_vec3(const Shader shader, const char* name, const float x, const float y, const float z);
void shader_set_vec4(const Shader shader, const char* name, const float x, const float y, const float z, const float w);
void shader_set_mat4(const Shader shader, const char* name, const float* mat);
void shader_set_mat3(const Shader shader, const char* name, const float* mat);

#endif //SHADER_H
