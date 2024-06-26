#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include "shader.h"

// Function to read the entire contents of a file into a const char*
const char* shader_read_file(const char* filename) {
    FILE* file = fopen(filename, "rb"); // Open file in binary mode for reading
    if (!file) {
        fprintf(stderr, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s\n", filename);
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer for the file content
    char* buffer = (char*)malloc(file_size + 1); // +1 for null-terminator
    if (!buffer) {
        fprintf(stderr, "ERROR::SHADER::MEMORY_ALLOCATION_FAILED\n");
        fclose(file);
        return NULL;
    }

    // Read file into buffer
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0'; // Null-terminate the string

    fclose(file);

    return buffer; // Note: the caller is responsible for freeing this memory
}

unsigned int shader_compile(const char* code, const GLenum type)
{
    const unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
        {
            printf("ERROR::SHADER:COMPILATION_FAILED:VERTEX");
        }
        else
        {
            printf("ERROR::SHADER:COMPILATION_FAILED:FRAGMENT");
        }

        printf(infoLog);
    }

    return shader;
}

unsigned int shader_create_program(const unsigned int vertex_shader, const unsigned int fragment_shader)
{
    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED");
        printf(infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shaderProgram;
}

Shader shader_new(const char* vertex_file_path, const char* fragment_file_path)
{
    Shader shader;

    const char* vertex_code = shader_read_file(vertex_file_path);
    const unsigned int vertex_shader = shader_compile(vertex_code, GL_VERTEX_SHADER);

    const char* fragment_code = shader_read_file(fragment_file_path);
    const unsigned int fragment_shader = shader_compile(fragment_code, GL_FRAGMENT_SHADER);

    shader.ID = shader_create_program(vertex_shader, fragment_shader);

    return shader;
}

void shader_use(const Shader shader)
{
    glUseProgram(shader.ID);
}

void shader_set_int(const Shader shader, const char* name, const int value)
{
    glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void shader_set_bool(const Shader shader, const char* name, const int value)
{
    glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void shader_set_float(const Shader shader, const char* name, const float value)
{
    glUniform1f(glGetUniformLocation(shader.ID, name), value);
}

void shader_set_vec3(const Shader shader, const char* name, const float x, const float y, const float z)
{
    glUniform3f(glGetUniformLocation(shader.ID, name), x, y, z);
}

void shader_set_vec4(const Shader shader, const char* name, const float x, const float y, const float z, const float w)
{
    glUniform4f(glGetUniformLocation(shader.ID, name), x, y, z, w);
}

void shader_set_mat4(const Shader shader, const char* name, const float* mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, name), 1, GL_FALSE, mat);
}