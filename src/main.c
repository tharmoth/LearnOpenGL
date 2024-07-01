#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "cglm/cglm.h"
#include "camera.h"
#include "world.h"
#include "cube.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, float delta);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float cutoff = 12.5f;

int main(void) {
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const Mesh cubeMesh = mesh_process(
        cube_pos,
        cube_normal,
        cube_uvs,
        sizeof(cube_pos) / sizeof(cube_pos[0]),
        cube_indices,
        sizeof(cube_indices) / sizeof(cube_indices[0]));
    const Shader cubeShader = shader_new("shaders/shader.vert", "shaders/shader.frag");

    World world = world_generate();
    vec3 directionalLightDirection = {-0.2f, -1.0f, -0.3f};
    camera = camera_new();

    float oldTime = glfwGetTime();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        const float newTime = glfwGetTime();
        const float delta = newTime - oldTime;
        oldTime = newTime;

        // input
        // -----
        processInput(window, delta);

        // Rendering commands
        // ------------------
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update Camera
        // -------------
        mat4 view;
        camera_view_matrix(&camera, view);
        shader_set_mat4(cubeShader, "view", view);

        mat4 projection;
        glm_perspective(glm_rad(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f, projection);
        shader_set_mat4(cubeShader, "projection", projection);

        // Draw Chunk
        // ----------
        mat4 viewInv;
        glm_mat4_inv(view, viewInv);
        glm_mat4_transpose(viewInv);

        shader_use(cubeShader);
        shader_set_mat4(cubeShader, "view", view);
        shader_set_mat4(cubeShader, "projection", projection);
        shader_set_float(cubeShader, "material.shininess", 10.0f);

        // Setup Directional Light
        // -----------------------
        vec3 directionalLightDirectionView;
        glm_mat4_mulv3(viewInv, directionalLightDirection, 1, directionalLightDirectionView);
        shader_set_vec3(cubeShader,
            "directionalLight.direction",
            directionalLightDirectionView[0],
            directionalLightDirectionView[1],
            directionalLightDirectionView[2]
            );
        shader_set_vec3(cubeShader, "directionalLight.ambient", 0.2f, 0.2f, 0.2f);
        shader_set_vec3(cubeShader, "directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
        shader_set_vec3(cubeShader, "directionalLight.specular", 0.05f, 0.05f, 0.05f);

        // Draw Cubes
        // ----------
        for (int i = 0; i < world.size; i++)
        {
            mat4 cubeModel;
            glm_mat4_identity(cubeModel);
            glm_translate(cubeModel, world.data[i]);

            mat3 normal;
            mat4 normalTransform;
            glm_mat4_mul(cubeModel, view, normalTransform);
            glm_mat4_inv(normalTransform, normalTransform);
            glm_mat4_transpose(normalTransform);
            glm_mat4_pick3(normalTransform, normal);

            shader_set_mat3(cubeShader, "normal", normal);
            shader_set_mat4(cubeShader, "model", cubeModel);
            mesh_draw(&cubeMesh, cubeShader);
        }

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


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, const float delta)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_process_keyboard(&camera, LEFT, delta);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_process_keyboard(&camera, RIGHT, delta);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_process_keyboard(&camera, BACKWARD, delta);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_process_keyboard(&camera, FORWARD, delta);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cutoff = glm_clamp(cutoff + 45.0f * delta, 0.0, 360.0f);
        printf("Cutoff: %f\n", cutoff);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cutoff = glm_clamp(cutoff - 45.0f * delta, 0.0, 360.0f);
        printf("Cutoff: %f\n", cutoff);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera_process_mouse(&camera, xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // camera_process_scroll(&camera, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}