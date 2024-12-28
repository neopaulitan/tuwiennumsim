#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <string.h>
#include <random>

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
double generateRandomNumber(double min, double max) {
    std::random_device rd;  // Hardware-Generator
    std::mt19937 gen(rd()); // Mersenne-Twister-Generator
    std::uniform_real_distribution<> dis(min, max); // Uniform distribution in [min, max]
    return dis(gen);
}