#pragma once

#include <cassert>
#include <iostream>

// The 'vec4' class represents a 4D vector and provides operations such as scaling, addition, subtraction, 
// normalization, and vector products (dot and cross).
class vec4 {
private:
    // Fast Inverse Square Root Implementation
    // This algorithm is best known for its usage at Quake III Arena (1999).
    // Original author(s) of this algorithm are Greg Walsh, and Cleve Moler.
    // This implementation is based on the mathematical approach and optimization done by Chris Lomont.
    // Reference(s):
    // Lomont, C. (2003). Fast Inverse Square Root:
    // https://www.lomont.org/papers/2003/InvSqrt.pdf
    // Historical context and attribution investigated by Rys Sommerfeldt:
    // https://www.beyond3d.com/content/articles/8/
    // https://www.beyond3d.com/content/articles/15/
    float Q_rsqrt(float x) {
        float xhalf = 0.5f * x;
        int i = *(int*)&x;                 // Get bits for floating value
        i = 0x5f375a86 - (i >> 1);         // Initial guess
        x = *(float*)&i;                   // Convert bits back to float
        x = x * (1.5f - (xhalf * x * x));  // Newton step, repeat to increase the accuracy
        // x = x * (1.5f - (xhalf * x * x));
        return x;
    }

public:
    union {
        struct { float x, y, z, w; };  // Components of the vector
        float v[4];                    // Array representation of the vector components
    };

    // Constructor to initialize the vector with specified values.
    // Default values: x = 0, y = 0, z = 0, w = 1.
    // Input Variables:
    // - _x: X component of the vector
    // - _y: Y component of the vector
    // - _z: Z component of the vector
    // - _w: W component of the vector (default is 1.0)
    vec4(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _w = 1.f) : x(_x), y(_y), z(_z), w(_w) {}

    // Displays the components of the vector in a readable format.
    void display() const {
        std::cout << x << '\t' << y << '\t' << z << '\t' << w << std::endl;
    }

    // Scales the vector by a scalar value.
    // Input Variables:
    // - scalar: Value to scale the vector by
    // Returns a new scaled 'vec4'.
    vec4 operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar, w * scalar };
    }

    // Divides the vector by its W component and sets W to 1.
    // Useful for normalizing the W component after transformations.
    void divideW() {
        #if OPT_VEC4_DISABLE_REDUNDANT_DIVS
            // Optimisation - 1 division, 3 multiplication
            float invW = 1.f / w;
            x *= invW; y *= invW; z *= invW; 
        #else
            // Base Rasterizer - 3 divisions...
            x /= w; y /= w; z /= w;
        #endif
        w = 1.f;  // Same assignment in both conditions, no need for code-line duplications
    }

    // Accesses a vector component by index.
    // Input Variables:
    // - index: Index of the component (0 for x, 1 for y, 2 for z, 3 for w)
    // Returns a reference to the specified component.
    float& operator[](const unsigned int index) {
        assert((index >= 0 && index <= 3) && "There is no such vector component!");
        return v[index];
    }

    // Accesses a vector component by index (const version).
    // Input Variables:
    // - index: Index of the component (0 for x, 1 for y, 2 for z, 3 for w)
    // Returns the specified component value.
    float operator[](const unsigned int index) const {
        assert((index >= 0 && index <= 3) && "There is no such vector component!");
        return v[index];
    }

    // Subtracts another vector from this vector.
    // Input Variables:
    // - other: The vector to subtract
    // Returns a new 'vec4' resulting from the subtraction.
    vec4 operator-(const vec4& other) const {
        return vec4(x - other.x, y - other.y, z - other.z, 0.0f);
    }

    // Adds another vector to this vector.
    // Input Variables:
    // - other: The vector to add
    // Returns a new 'vec4' resulting from the addition.
    vec4 operator+(const vec4& other) const {
        return vec4(x + other.x, y + other.y, z + other.z, 0.0f);
    }

    // Computes the cross product of two vectors.
    // Input Variables:
    // - v1: The first vector
    // - v2: The second vector
    // Returns a new 'vec4' representing the cross product.
    static vec4 cross(const vec4& v1, const vec4& v2) {
        return vec4(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x,
            0.0f  // The W component is set to 0 for cross products
        );
    }

    // Computes the dot product of two vectors.
    // Input Variables:
    // - v1: The first vector
    // - v2: The second vector
    // Returns the dot product as a float.
    static float dot(const vec4& v1, const vec4& v2) {
        return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    }

    // Normalizes the vector to make its length equal to 1.
    // This operation does not affect the W component.
    void normalise() {
        #if OPT_VEC4_DISABLE_REDUNDANT_DIVS
            // Optimisation - 1 Division, 3 Multiplication
            float length = std::sqrt(x * x + y * y + z * z);
            float invLength = 1.f / length;
            x *= invLength; y *= invLength; z *= invLength;
        #elif OPT_VEC4_FAST_INV_SQRT
            // Optimisation - Quake III Arena Fast Inverse Square Root (in fact, this slows down the program on modern CPUs...)
            float invLength = Q_rsqrt(x * x + y * y + z * z);
            x *= invLength; y *= invLength; z *= invLength;
        #else
            // Base Rasterizer - 3 Divisions...
            float length = std::sqrt(x * x + y * y + z * z);
            x /= length; y /= length; z /= length;
        #endif
    }
};