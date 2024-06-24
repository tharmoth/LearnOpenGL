#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shader.h"
#include "texture.h"
#include "vertex.h"
#include "cglm/cglm.h"


float mix = 0.0f;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, const float delta)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mix = fmin(mix + 1.0 * delta, 1.0f);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mix = fmax(mix - 1.0 * delta, 0.0f);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main(void)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, 800, 600);

    const float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
   };
    const unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    const Vertex vertex = vertex_new(vertices, sizeof(vertices), indices, sizeof(indices));

    const Shader shader = shader_new("shaders/shader.vs", "shaders/shader.fs");
    shader_use(shader);
    shader_set_int(shader, "texture1", 0);
    shader_set_int(shader, "texture2", 1);

    const Texture texture_one = texture_new("data/container.jpg", GL_RGB);
    const Texture texture_two = texture_new("data/awesomeface.png", GL_RGBA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_one.ID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_two.ID);


    float rotation = 0;
    float scale = 1;
    float time = glfwGetTime();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        const float new_time = glfwGetTime();
        const float delta = new_time - time;
        time = new_time;

        // input
        // -----
        processInput(window, delta);

        // Rendering commands
        // ------------------
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the first square
        rotation += 90 * delta;
        mat4 trans = {
            1.0f, 0, 0, 0,
            0, 1.0f, 0, 0,
            0, 0, 1.0f, 0,
            0, 0, 0, 1.0f
          };
        glm_translate(trans, (vec3) {0.5f, -0.5f, 0.0f});
        glm_rotate(trans, glm_rad(rotation), (vec3) {0.0f, 0.0f, 1.0f}) ;
        glm_scale(trans, (vec3) {0.5, 0.5, 0.5});
        shader_set_mat4(shader, "transform", trans);
        vertex_draw(vertex);

        // Draw the second square
        scale = sin(time) / 2.0f + 1.0;
        mat4 trans2 = {
            1.0f, 0, 0, 0,
            0, 1.0f, 0, 0,
            0, 0, 1.0f, 0,
            0, 0, 0, 1.0f
          };
        glm_translate(trans2, (vec3) {-0.5f, 0.5f, 0.0f});
        glm_scale(trans2, (vec3) {scale, scale, scale});
        shader_set_mat4(shader, "transform", trans2);
        vertex_draw(vertex);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}