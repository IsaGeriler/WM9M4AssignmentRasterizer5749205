#pragma once

#include "colour.h"
#include "vec4.h"

// Keep light straightforward - struct for storing information
struct Light {
    vec4 omega_i;    // light direction
    colour L;        // light colour
    colour ambient;  // ambient light component 

    // Optimisation Idea - Consider converting AoS to SoA...
};