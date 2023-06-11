#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <limits>

const float INF = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897932385f;

inline float degrees2radian(float degrees) {
    return degrees * PI / 180;
}

inline float random_float() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
    //static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    //static std::mt19937 generator;
    //return distribution(generator);
}

inline float random_float(float min, float max) {
    // Returns a random real in [0,1).
    return min + random_float() * (max - min);
}

inline int random_int(int min, int max) {
    // Returns a random int in [min, max]
    return int(random_float(min, max + 1));
}

#endif // !UTILS_H
