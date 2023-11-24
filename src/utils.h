//
// Created by UNIkeEN on 23-11-23.
//

#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>

// Constants

const double PI = 3.1415926535897932;
const double INF = std::numeric_limits<double>::infinity();
const double EPSILON = 0.001;

// Utility Functions

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
};

inline double deg2rad(double degrees) {
    return degrees * PI / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

void show_progress(int current, int total) {
    const int bar_width = 70;
    float progress = (float)current / total;
    int pos = bar_width * progress;

    printf("\r[");
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d/%d (%6.2f%%)", current, total, progress * 100.0);
    fflush(stdout);
}