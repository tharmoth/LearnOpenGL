//
// Created by Tharmoth on 6/26/2024.
//

#include "noise.h"

#include <math.h>

#include "noise/noise1234.h"

float noise_generate(const float x, const float y, const float wavelength, const int octaves, const int seed)
{
    float noise = 0;
    for (int i = 1; i <= octaves; i++)
    {
        const float octive_contribution = 1.0f / i;
        const float octive_wavelength = wavelength / i;

        noise += octive_contribution * (noise3(x / octive_wavelength + 0.1f , y  / octive_wavelength + 0.1f, seed) + 1.0) / 2.0f;
    }

    // Renormalise
    float sum = 0;
    for (int i = 1; i <= octaves; i++)
    {
        sum += 1.0f / i;
    }
    noise /= sum;

    // Jank Scaling I don't understand to get it to go from [~1, ~0] with 1 octive.
    noise -= 0.223298f;
    noise *= 1.0f / 0.552027f;

    // noise = pow(noise, 10.0f);

    return noise;
}