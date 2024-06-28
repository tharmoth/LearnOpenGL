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
#include "camera.h"
#include "glfw/src/internal.h"
#include "noise.h";




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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const float cubeVertices[] = {
        // Position           // Normal            // UV
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Face 1
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // Face 2
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Face 3
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // Face 4
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // Face 5
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // Face 6
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    const Vertex vertex = vertex_new(cubeVertices, sizeof(cubeVertices));
    const Vertex lightVertex = vertex_new(cubeVertices, sizeof(cubeVertices));

    const Shader cubeShader = shader_new("shaders/shader.vert", "shaders/shader.frag");
    const Shader lightShader = shader_new("shaders/shader.vert", "shaders/lightShader.frag");

    const Texture textureDiffuse = texture_new("data/dirt.png", GL_RGBA);
    const Texture textureSpecular = texture_new("data/container2_specular.png", GL_RGBA);

    texture_bind(&textureDiffuse, 0);
    texture_bind(&textureSpecular, 1);

    const unsigned int xSize = 64;
    const unsigned int ySize = 16;
    const unsigned int zSize = 64;

    float max = FLT_MIN;
    float min = FLT_MAX;
    vec3 chunk[xSize * ySize * zSize];
    int index = 0;
    float wavelength = 16.0f;
    int seed = 10;
    for(int z = 0; z < zSize; z++)
    {
        for(int y = 0; y < ySize; y++)
        {
            for(int x = 0; x < xSize; x++)
            {
                float max_height = (noise_generate(x, z, 16, 5, seed));

                if (max_height > max)
                {
                    max = max_height;
                }
                if (max_height < min)
                {
                    min = max_height;
                }

                max_height *= ySize;

                if (y < max_height)
                {
                    chunk[index][0] = (float) x;
                    chunk[index][1] = (float) y - ySize;
                    chunk[index][2] = (float) z;
                }
                else
                {
                    chunk[index][0] = -INT_MIN;
                    chunk[index][1] = -INT_MIN;
                    chunk[index][2] = -INT_MIN;
                }

                index++;
            }
        }
    }

    vec3 pointLightPositions[] = {
        {16.0f, -2.0f, 16.0f},
        {16.0f, -2.0f, 32.0f},
        {32.0f, -2.0f, 16.0f},
        {32.0f, -2.0f, 32.0f},
    };
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

        shader_set_int(cubeShader, "material.diffuse", 0);
        shader_set_int(cubeShader, "material.specular", 1);
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
        shader_set_vec3(cubeShader, "directionalLight.ambient", 0.05f, 0.05f, 0.05f);
        shader_set_vec3(cubeShader, "directionalLight.diffuse", 0.1f, 0.1f, 0.1f);
        shader_set_vec3(cubeShader, "directionalLight.specular", 0.05f, 0.05f, 0.05f);

        // Setup Point Lights
        // ------------------
        char uniformName[50];
        for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++)
        {
            vec3 pointLightPosView;
            glm_mat4_mulv3(view, pointLightPositions[i], 1, pointLightPosView);
            sprintf(uniformName, "pointLights[%d].position", i);
            shader_set_vec3(cubeShader, uniformName, pointLightPosView[0], pointLightPosView[1], pointLightPosView[2]);
            sprintf(uniformName, "pointLights[%d].ambient", i);
            shader_set_vec3(cubeShader, uniformName, 0.05f, 0.05f, 0.05f);
            sprintf(uniformName, "pointLights[%d].diffuse", i);
            shader_set_vec3(cubeShader, uniformName, 0.8f, 0.8f, 0.8f);
            sprintf(uniformName, "pointLights[%d].specular", i);
            shader_set_vec3(cubeShader, uniformName, 1.0f, 1.0f, 1.0f);
            sprintf(uniformName, "pointLights[%d].constant", i);
            shader_set_float(cubeShader, uniformName, 1.0f);
            sprintf(uniformName, "pointLights[%d].linear", i);
            shader_set_float(cubeShader, uniformName, 0.09f);
            sprintf(uniformName, "pointLights[%d].quadratic", i);
            shader_set_float(cubeShader, uniformName, 0.032f);
        }

        // Setup Spot Light
        // ----------------
        vec3 spotLightPosView;
        glm_mat4_mulv3(view, camera.position, 1, spotLightPosView);

        vec3 spotLightDirView;
        glm_mat4_mulv3(viewInv, camera.front, 1, spotLightDirView);

        shader_set_vec3(cubeShader, "spotLight.position", spotLightPosView[0], spotLightPosView[1], spotLightPosView[2]);
        shader_set_vec3(cubeShader, "spotLight.direction", spotLightDirView[0], spotLightDirView[1], spotLightDirView[2]);

        shader_set_float(cubeShader, "spotLight.innerCutoff", glm_rad(cutoff - 10));
        shader_set_float(cubeShader, "spotLight.outerCutoff", glm_rad(cutoff));
        shader_set_vec3(cubeShader, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
        shader_set_vec3(cubeShader, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        shader_set_vec3(cubeShader, "spotLight.specular", 1.0f, 1.0f, 1.0f);

        shader_set_float(cubeShader, "spotLight.constant", 1.0f);
        shader_set_float(cubeShader, "spotLight.linear", 0.09f);
        shader_set_float(cubeShader, "spotLight.quadratic", 0.032f);

        // Draw Cubes
        // ----------
        for (int i = 0; i < sizeof(chunk) / sizeof(chunk[0]); i++)
        {
            mat4 cubeModel;
            glm_mat4_identity(cubeModel);
            glm_translate(cubeModel, chunk[i]);

            mat3 normal;
            mat4 normalTransform;
            glm_mat4_mul(cubeModel, view, normalTransform);
            glm_mat4_inv(normalTransform, normalTransform);
            glm_mat4_transpose(normalTransform);
            glm_mat4_pick3(normalTransform, normal);

            shader_set_mat3(cubeShader, "normal", normal);
            shader_set_mat4(cubeShader, "model", cubeModel);
            vertex_draw(vertex);
        }

        // Draw Light Cubes
        // ----------------
        for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++)
        {
            mat4 lightModel;
            glm_mat4_identity(lightModel);
            glm_translate(lightModel, pointLightPositions[i]);
            glm_scale(lightModel, (vec3) {0.2f, .2f, 0.2f});

            shader_use(lightShader);
            shader_set_mat4(lightShader, "view", view);
            shader_set_mat4(lightShader, "projection", projection);
            shader_set_mat4(lightShader, "model", lightModel);
            shader_set_vec3(lightShader, "color", 1.0f, 1.0f, 1.0f);

            vertex_draw(lightVertex);
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
