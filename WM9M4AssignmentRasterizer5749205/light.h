#pragma once

#include <vector>

#include "colour.h"
#include "vec4.h"

// Keep light straightforward - struct for storing information
struct Light {
    vec4 omega_i;    // light direction
    colour L;        // light colour
    colour ambient;  // ambient light component
};