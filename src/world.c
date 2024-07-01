//
// Created by tharmoth on 7/1/2024.
//

#include "world.h"

#include "noise.h";

void world_free(World* world) {
    free(world->data);
}

World world_generate() {
    const unsigned int xSize = 64;
    const unsigned int ySize = 16;
    const unsigned int zSize = 64;

    float max = FLT_MIN;
    float min = FLT_MAX;
    vec3* chunk = malloc(xSize * ySize * zSize * sizeof(vec3));
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

    const World world = {
        .size = xSize * ySize * zSize,
        .data = chunk
    };
    return world;
}