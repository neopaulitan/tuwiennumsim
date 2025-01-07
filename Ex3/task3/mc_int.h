#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <omp.h>
#include <mutex>
#include <chrono>
#include <cmath>

float sinx(float x) {
    return sin(x);
}

float cos2xinv(float x) {
    return pow(cos(1/x), 2);
}

float x4m5(float x) {
    return 5*pow(x, 4);
}

// Fgenerates a random number in the interval [min, max].
double generateRandomNumber(double min, double max, std::mt19937 &gen) {
    std::uniform_real_distribution<> dis(min, max); // Uniform distribution in [min, max]
    return dis(gen);
}