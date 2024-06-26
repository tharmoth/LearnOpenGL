//
// Created by Tharmoth on 6/24/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "cglm/cglm.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

typedef struct
{
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;

    float speed;

    float sensitivity;
    float pitch;
    float yaw;

    float zoom;
}Camera;

Camera camera_new();
void camera_view_matrix(Camera* camera, mat4 viewMatrix);
void camera_process_keyboard(Camera* camera, enum Camera_Movement direction, float deltaTime);
void camera_process_mouse(Camera* camera, float xoffset, float yoffset);
void camera_process_scroll(Camera* camera, float yoffset);

#endif //CAMERA_H
