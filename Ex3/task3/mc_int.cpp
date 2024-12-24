#include "mc_int.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 5) {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    char* function = argv[1];
    double xmin = atof(argv[2]);
    double xmax = atof(argv[3]);
    int samples = atoi(argv[4]);

    std::cout << "You provided the following arguments:\n" << std::endl;
    std::cout << "Function: " << function << std::endl;
    std::cout << "Xmin: " << xmin << std::endl;
    std::cout << "Xmax: " << xmax << std::endl;
    std::cout << "Samples: " << samples << std::endl;

    // Function pointer to the function to be integrated.
    float (*f_val)(float) = NULL;

    if (strcmp(function, "sinx") == 0) {
        f_val = sinx;
    } else if (strcmp(function, "cos2xinv") == 0) {
        f_val = cos2xinv;
    } else if (strcmp(function, "x4m5") == 0) {
        f_val = x4m5;
    } else {
        std::cout << "Invalid function name!" << std::endl;
        return 1;
    }

    srand(time(NULL));

    double inside = 0.0;

    // Checks for each random pair of x and y if they are inside circle of radius 1.
    for (int i = 1; i < samples; i++) {
        double randX = generateRandomNumber(xmin, xmax);
        double randY = generateRandomNumber(xmin, xmax);
        if (randY <= f_val(randX)) {
            inside++;
        }
    }

    // Calculating pi and printing.
    inside *= (xmax - xmin) / samples;

    std::cout << "Integral: " << inside << std::endl;
    return 0;
}