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

    std::string func_str(function);
    std::transform(func_str.begin(), func_str.end(), func_str.begin(), ::tolower);

    if (func_str == "sinx") {
        f_val = sinx;
    } else if (func_str == "cos2xinv") {
        f_val = cos2xinv;
    } else if (func_str == "x4m5") {
        f_val = x4m5;
    } else {
        std::cout << "Invalid function name!" << std::endl;
        return 1;
    }

    srand(time(NULL));

    double integral = 0.0;

    // Calculating integral sum.
    for (int k = 1; k <= samples; k++) {
        double randX = generateRandomNumber(xmin, xmax);
        integral += f_val(randX);
    }

    // Calculating mean.
    integral *= (xmax - xmin) / samples;

    std::cout << "Integral: " << integral << std::endl;
    return 0;
}