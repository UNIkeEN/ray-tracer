// UNIkeEN 2023/11/23

#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>

// Constants

const double PI = 3.1415926535897932;
const double INF = std::numeric_limits<double>::infinity();
const double EPSILON = 0.0001; 

// Utility Functions

inline double deg2rad(double degrees) {
    return degrees * PI / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}