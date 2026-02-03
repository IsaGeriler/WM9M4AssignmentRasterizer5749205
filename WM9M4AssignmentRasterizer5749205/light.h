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

struct LightSoA {
    // Light Direction Components
    std::vector<float> omega_ix;
    std::vector<float> omega_iy;
    std::vector<float> omega_iz;
    std::vector<float> omega_iw;

    // Light Colour Components
    std::vector<float> L_r;
    std::vector<float> L_g;
    std::vector<float> L_b;

    // Ambient Light Components
    std::vector<float> ambient_r;
    std::vector<float> ambient_g;
    std::vector<float> ambient_b;
};