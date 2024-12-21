#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
//#include <openmp.h>

float sinx(float x) {
    return sin(x);
}

float cos2xinv(float x) {
    return pow(cos(1/x), 2);
}

float x4m5(float x) {
    return pow(5*x, 4);
}

int main(int argc, char *argv[]) {
    char* function = argv[1];
    double xmin = atof(argv[2]);
    double xmax = atof(argv[3]);
    int samples = atoi(argv[4]);

    printf("You provided the following arguments:\n");
    printf("Function: %s\n", function);
    printf("Xmin: %.2lf\n", xmin);
    printf("Xmax: %.2lf\n", xmax);
    printf("Samples: %d\n", samples);

    // Function pointer to the function to be integrated.
    float (*f_val)(float) = NULL;

    if (strcmp(function, "sinx") == 0) {
        f_val = sinx;
    } else if (strcmp(function, "cos2xinv") == 0) {
        f_val = cos2xinv;
    } else if (strcmp(function, "x4m5") == 0) {
        f_val = x4m5;
    } else {
        printf("Invalid function name!\n");
        return 1;
    }

    srand(time(NULL));

    int inside = 0;

    // Checks for each random pair of x and y if they are inside circle of radius 1.
    for (int i = 1; i < samples; i++) {
        double randX = xmin + (xmax - xmin) * (double) rand() / RAND_MAX;
        double randY = xmin + (xmax - xmin) * (double) rand() / RAND_MAX;
        if (randY <= f_val(randX)) {
            inside++;
        }
    }

    // Calculating pi and printing.
    inside *= (xmax - xmin) / samples;

    printf("Samples: %d\n", samples);
    printf("Integral: %f\n", inside);
    return 0;
}