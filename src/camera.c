//
// Created by Tharmoth on 6/24/2024.
//

#include "camera.h"

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

void camera_update_vectors(Camera* camera)
{
    camera->front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    camera->front[1] = sin(glm_rad(camera->pitch));
    camera->front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(camera->front);

    glm_cross(camera->front, camera->worldUp, camera->right);
    glm_normalize(camera->right);
    glm_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);
}

Camera camera_new()
{
    Camera camera = {
        .sensitivity = SENSITIVITY,
        .speed = SPEED,
        .worldUp = {0.0f, 1.0f, 0.0f},
        .position = {0.0f, 0.0f, 3.0f},
        .zoom = ZOOM,
        .pitch = PITCH,
        .yaw = YAW
    };

    camera_update_vectors(&camera);

    return camera;
}

void camera_view_matrix(Camera* camera, mat4 viewMatrix)
{
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, viewMatrix);
}

void camera_process_keyboard(Camera* camera, enum Camera_Movement direction, float deltaTime)
{
    const float velocity = camera->speed * deltaTime;
    vec3 movement;
    if(direction == LEFT)
    {
        glm_vec3_scale(camera->right, -velocity, movement);
    }
    if(direction == RIGHT)
    {
        glm_vec3_scale(camera->right, velocity, movement);
    }
    if(direction == BACKWARD)
    {
        glm_vec3_scale(camera->front, -velocity, movement);
    }
    if(direction == FORWARD)
    {
        glm_vec3_scale(camera->front, velocity, movement);
    }
    glm_vec3_add(camera->position, movement, camera->position);
}

void camera_process_mouse(Camera* camera, float xoffset, float yoffset)
{
    float xoff = xoffset * camera->sensitivity;
    float yoff = -yoffset * camera->sensitivity;

    camera->yaw   += xoff;
    camera->pitch -= yoff;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (true)
    {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    camera_update_vectors(camera);
}

void camera_process_scroll(Camera* camera, float yoffset)
{
    camera->zoom -= yoffset;
    if (camera->zoom < 1.0f)
        camera->zoom = 1.0f;
    if (camera->zoom > 45.0f)
        camera->zoom = 45.0f;
}


