#pragma once

// Z-buffer/Depth Buffer Class Optimisation
#define OPT_DEPTH_BUFFER_CLEAR_AVX2 true

// Colour Class Optimisation
#define OPT_COLOUR_DISABLE_FLOOR true

// Matrix Class Optimisations
#define OPT_MATRIX_VECMUL_AVX true
#define OPT_MATRIX_UNROLLMUL false
#define OPT_MATRIX_4X4MUL_AVX true
#define OPT_MATRIX_PERSPECTIVE_DIV true
#define OPT_MATRIX_TRIG_CALLS true
#define OPT_MATRIX_DISABLE_REDUNDANT_IDENTITY_CALL true
#define OPT_MATRIX_INITS true

// Renderer Class Optimisation
#define OPT_RENDERER_DISABLE_REDUNDANT_DIVS true

// Vec4 Class Optimisation
#define OPT_VEC4_DISABLE_REDUNDANT_DIVS true
#define OPT_VEC4_FAST_INV_SQRT false

// Triangle Class Optimisation
#define OPT_TRIANGLE_BACKFACE_CULLING true
#define OPT_TRIANGLE_INV_AREA true
#define OPT_TRIANGLE_SPLIT_BARYCENTRIC_CHECKS true
#define OPT_TRIANGLE_EARLY_DEPTH_CHECK true
#define OPT_TRIANGLE_EARLY_LIGHT_NORM true

// Raster.cpp Optimisation
#define OPT_RASTER_ENABLE_VERTEX_CACHING true