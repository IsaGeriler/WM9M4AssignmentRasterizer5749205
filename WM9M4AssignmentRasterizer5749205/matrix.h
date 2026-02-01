#pragma once

#include <iostream>
#include <immintrin.h>
#include <vector>

#include "vec4.h"

// Matrix class for 4x4 transformation matrices
class matrix {
    union {
        float m[4][4];  // 2D array representation of the matrix
        float a[16];    // 1D array representation of the matrix for linear access
    };

public:
    // Default constructor initializes the matrix as an identity matrix
    matrix() {
        identity();
    }

    // Access matrix elements by row and column
    float& operator()(unsigned int row, unsigned int col) {
        assert((row >= 0 && row <= 3) && "There is no such row component!");
        assert((col >= 0 && col <= 3) && "There is no such column component!");
        return m[row][col];
    }

    // Display the matrix elements in a readable format
    void display() const {
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++)
                std::cout << m[i][j] << '\t';
            std::cout << std::endl;
        }
    }

    // Multiply the matrix by a 4D vector
    // Input Variables:
    // - v: vec4 object to multiply with the matrix
    // Returns the resulting transformed vec4
    vec4 operator * (const vec4& v) const {
        vec4 result;
        result[0] = a[0] * v[0] + a[1] * v[1] + a[2] * v[2] + a[3] * v[3];
        result[1] = a[4] * v[0] + a[5] * v[1] + a[6] * v[2] + a[7] * v[3];
        result[2] = a[8] * v[0] + a[9] * v[1] + a[10] * v[2] + a[11] * v[3];
        result[3] = a[12] * v[0] + a[13] * v[1] + a[14] * v[2] + a[15] * v[3];
        return result;
    }

    // Multiply the matrix by another matrix
    // Input Variables:
    // - mx: Another matrix to multiply with
    // Returns the resulting matrix
    matrix operator*(const matrix& mx) const {
        matrix ret;

        // Base Rasterizer
        //for (int row = 0; row < 4; ++row) {
        //    for (int col = 0; col < 4; ++col) {
        //        ret.a[row * 4 + col] =
        //            a[row * 4 + 0] * mx.a[0 * 4 + col] +
        //            a[row * 4 + 1] * mx.a[1 * 4 + col] +
        //            a[row * 4 + 2] * mx.a[2 * 4 + col] +
        //            a[row * 4 + 3] * mx.a[3 * 4 + col];
        //    }
        //}

        // Optimized - Unrolling & Hardcoding Results
        //ret.a[0] = a[0] * mx.a[0] + a[1] * mx.a[4] + a[2] * mx.a[8] + a[3] * mx.a[12];
        //ret.a[1] = a[0] * mx.a[1] + a[1] * mx.a[5] + a[2] * mx.a[9] + a[3] * mx.a[13];
        //ret.a[2] = a[0] * mx.a[2] + a[1] * mx.a[6] + a[2] * mx.a[10] + a[3] * mx.a[14];
        //ret.a[3] = a[0] * mx.a[3] + a[1] * mx.a[7] + a[2] * mx.a[11] + a[3] * mx.a[15];

        //ret.a[4] = a[4] * mx.a[0] + a[5] * mx.a[4] + a[6] * mx.a[8] + a[7] * mx.a[12];
        //ret.a[5] = a[4] * mx.a[1] + a[5] * mx.a[5] + a[6] * mx.a[9] + a[7] * mx.a[13];
        //ret.a[6] = a[4] * mx.a[2] + a[5] * mx.a[6] + a[6] * mx.a[10] + a[7] * mx.a[14];
        //ret.a[7] = a[4] * mx.a[3] + a[5] * mx.a[7] + a[6] * mx.a[11] + a[7] * mx.a[15];

        //ret.a[8] = a[8] * mx.a[0] + a[9] * mx.a[4] + a[10] * mx.a[8] + a[11] * mx.a[12];
        //ret.a[9] = a[8] * mx.a[1] + a[9] * mx.a[5] + a[10] * mx.a[9] + a[11] * mx.a[13];
        //ret.a[10] = a[8] * mx.a[2] + a[9] * mx.a[6] + a[10] * mx.a[10] + a[11] * mx.a[14];
        //ret.a[11] = a[8] * mx.a[3] + a[9] * mx.a[7] + a[10] * mx.a[11] + a[11] * mx.a[15];

        //ret.a[12] = a[12] * mx.a[0] + a[13] * mx.a[4] + a[14] * mx.a[8] + a[15] * mx.a[12];
        //ret.a[13] = a[12] * mx.a[1] + a[13] * mx.a[5] + a[14] * mx.a[9] + a[15] * mx.a[13];
        //ret.a[14] = a[12] * mx.a[2] + a[13] * mx.a[6] + a[14] * mx.a[10] + a[15] * mx.a[14];
        //ret.a[15] = a[12] * mx.a[3] + a[13] * mx.a[7] + a[14] * mx.a[11] + a[15] * mx.a[15];

        // Optimized - AVX Multiplication (SIMD, using m128 registers (4 floats) instead of m256 (8 floats))
        // Important: A x B != B x A
        // Load the entirety of mx/right hand side matrix
        __m128 col_one = _mm_loadu_ps(&mx.m[0][0]);
        __m128 col_two = _mm_loadu_ps(&mx.m[1][0]);
        __m128 col_three = _mm_loadu_ps(&mx.m[2][0]);
        __m128 col_four = _mm_loadu_ps(&mx.m[3][0]);

        for (size_t i = 0; i < 4; i++) {
            // Load x, y, z, and w components of this/left hand side matrix
            __m128 x = _mm_broadcast_ss(&m[i][0]);
            __m128 y = _mm_broadcast_ss(&m[i][1]);
            __m128 z = _mm_broadcast_ss(&m[i][2]);
            __m128 w = _mm_broadcast_ss(&m[i][3]);

            // Calculate Dot Products
            __m128 resA = _mm_fmadd_ps(x, col_one, _mm_mul_ps(y, col_two));
            __m128 resB = _mm_fmadd_ps(z, col_three, _mm_mul_ps(w, col_four));

            // Store the result
            _mm_storeu_ps(&ret.m[i][0], _mm_add_ps(resA, resB));
        }

        return ret;
    }

    // Create a perspective projection matrix
    // Input Variables:
    // - fov: Field of view in radians
    // - aspect: Aspect ratio of the viewport
    // - n: Near clipping plane
    // - f: Far clipping plane
    // Returns the perspective matrix
    static matrix makePerspective(float fov, float aspect, float n, float f) {
        matrix m;
        m.zero();

        // Base Rasterizer - Division is an expensive operation
        //float tanHalfFov = std::tan(fov / 2.0f);
        //m.a[0] = 1.0f / (aspect * tanHalfFov);
        //m.a[5] = 1.0f / tanHalfFov;
        //m.a[10] = -f / (f - n);
        //m.a[11] = -(f * n) / (f - n);
        //m.a[14] = -1.0f;
        
        // Optimized - Reduced Division Count
        float tanHalfFov = std::tan(fov * 0.5f);
        float inverseAspectTanHalfFov = 1.f / (aspect * tanHalfFov);
        float negFOverZNormalized = -f / (f - n);

        m.a[0] = inverseAspectTanHalfFov;           // 1.f / aspect * tanHalfFov
        m.a[5] = inverseAspectTanHalfFov * aspect;  // 1.f / tanHalfFov
        m.a[10] = negFOverZNormalized;              // -f / (f - n)
        m.a[11] = negFOverZNormalized * n;          // -(f * n) / (f - n)
        m.a[14] = -1.f;
       
        return m;
    }

    // Create a translation matrix
    // Input Variables:
    // - tx, ty, tz: Translation amounts along the X, Y, and Z axes
    // Returns the translation matrix
    static matrix makeTranslation(float tx, float ty, float tz) {
        // Optimized - Default constructor already sets to identity matrix
        matrix m;
        // m.identity();
        m.a[3] = tx;
        m.a[7] = ty;
        m.a[11] = tz;
        return m;
    }

    // Create a rotation matrix around the Z-axis
    // Input Variables:
    // - aRad: Rotation angle in radians
    // Returns the rotation matrix
    static matrix makeRotateZ(float aRad) {
        // Optimized - Default constructor already sets to identity matrix
        matrix m;
        // m.identity();

        // Optimized - Store sin and cos to avoid multiple calculations
        float sinA = std::sin(aRad);
        float cosA = std::cos(aRad);

        m.a[0] = cosA;
        m.a[1] = -sinA;
        m.a[4] = sinA;
        m.a[5] = cosA;
        return m;
    }

    // Create a rotation matrix around the X-axis
    // Input Variables:
    // - aRad: Rotation angle in radians
    // Returns the rotation matrix
    static matrix makeRotateX(float aRad) {
        // Optimized - Default constructor already sets to identity matrix
        matrix m;
        // m.identity();

        // Optimized - Store sin and cos to avoid multiple calculations
        float sinA = std::sin(aRad);
        float cosA = std::cos(aRad);

        m.a[5] = cosA;
        m.a[6] = -sinA;
        m.a[9] = sinA;
        m.a[10] = cosA;
        return m;
    }

    // Create a rotation matrix around the Y-axis
    // Input Variables:
    // - aRad: Rotation angle in radians
    // Returns the rotation matrix
    static matrix makeRotateY(float aRad) {
        // Optimized - Default constructor already sets to identity matrix
        matrix m;
        // m.identity();

        // Optimized - Store sin and cos to avoid multiple calculations
        float sinA = std::sin(aRad);
        float cosA = std::cos(aRad);

        m.a[0] = cosA;
        m.a[2] = sinA;
        m.a[8] = -sinA;
        m.a[10] = cosA;
        return m;
    }

    // Create a composite rotation matrix from X, Y, and Z rotations
    // Input Variables:
    // - x, y, z: Rotation angles in radians around each axis
    // Returns the composite rotation matrix
    static matrix makeRotateXYZ(float x, float y, float z) {
        return matrix::makeRotateX(x) * matrix::makeRotateY(y) * matrix::makeRotateZ(z);
    }

    // Create a scaling matrix
    // Input Variables:
    // - s: Scaling factor
    // Returns the scaling matrix
    static matrix makeScale(float s) {
        // Optimized - Default constructor already sets to identity matrix
        matrix m;
        // m.identity();

        s = std::max(s, 0.01f);  // Ensure scaling factor is not too small
        m.a[0] = s;
        m.a[5] = s;
        m.a[10] = s;
        return m;
    }

    // Create an identity matrix
    // Returns an identity matrix
    static matrix makeIdentity() {
        matrix m;
        // Base Rasterizer
        //for (int i = 0; i < 4; ++i) {
        //    for (int j = 0; j < 4; ++j) {
        //        m.m[i][j] = (i == j) ? 1.0f : 0.0f;
        //    }
        //}

        // Optimise - SIMD Zero, Diagonals One
        m.zero();
        m.a[0] = 1.f; m.a[5] = 1.f; m.a[10] = 1.f; m.a[15] = 1.f;
        return m;
    }

private:
    // Set all elements of the matrix to 0
    void zero() {
        // Base Rasterizer
        //for (unsigned int i = 0; i < 16; i++)
        //    a[i] = 0.f;

        // Optimization - SIMD
        __m256 vzero = _mm256_setzero_ps();
        size_t i = 0;

        for (; i + 7 < 16; i += 8)
            _mm256_storeu_ps(&a[i], vzero);

        // Handle remaining elements (if any left)
        for (; i < 16; i++)
            a[i] = 0.f;
    }

    // Set the matrix as an identity matrix
    void identity() {
        // Base Rasterizer
        //for (int i = 0; i < 4; ++i) {
        //    for (int j = 0; j < 4; ++j) {
        //        m[i][j] = (i == j) ? 1.0f : 0.0f;
        //    }
        //}
        
        // Optimise - SIMD Zero, Diagonals One
        zero();
        a[0] = 1.f; a[5] = 1.f; a[10] = 1.f; a[15] = 1.f;
    }
};