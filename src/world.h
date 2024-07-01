//
// Created by tharmoth on 7/1/2024.
//

#ifndef WORLD_H
#define WORLD_H

#include "cglm/cglm.h"

typedef struct {
    unsigned int size;
    vec3* data;
} World;

World world_generate();
void world_free(World* chunk);

#endif //WORLD_H
