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
    return pow(5*x, 4);
}

// Funktion, um eine Zufallszahl im Intervall [xmin, xmax] zu erzeugen
double generateRandomNumber(double min, double max) {
    std::random_device rd;  // Zufallsgerät
    std::mt19937 gen(rd()); // Mersenne-Twister-Generator
    std::uniform_real_distribution<> dis(min, max); // Gleichverteilte Zufallszahlen im Intervall [min, max]
    return dis(gen);
}