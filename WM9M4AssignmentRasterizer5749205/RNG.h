#pragma once

#include <random>

class RandomNumberGenerator {
public:
    // Delete copy constructor and assignment operator
    RandomNumberGenerator(const RandomNumberGenerator&) = delete;
    RandomNumberGenerator& operator=(const RandomNumberGenerator&) = delete;

    // Get the singleton instance
    static RandomNumberGenerator& getInstance() {
        static RandomNumberGenerator instance;
        return instance;
    }

    // Generate a random integer within a range
    int getRandomInt(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(rng);
    }

    // Generate a random integer within a range
    float getRandomFloat(float min, float max) {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(rng);
    }

private:
    // Private constructor for Singleton
    RandomNumberGenerator() : rng(std::random_device{}()) {}

    // Mersenne Twister random number generator
    std::mt19937 rng;
};