#include "mc_int.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 5) {
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }

    char* function = argv[1];
    double xmin = strtod(argv[2], nullptr);
    double xmax = strtod(argv[3], nullptr);
    long samples = static_cast<long>(strtod(argv[4], nullptr));

    // Function pointer to the function to be integrated.
    float (*f_val)(float) = nullptr;

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

    std::mt19937 shared_rng(42); // fixed seed for reproducibility
    std::mutex rng_mutex;

    double global_integral = 0.0;

    auto start_time = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        unsigned int thread_seed;
        {
            std::lock_guard<std::mutex> lock(rng_mutex);
            thread_seed = shared_rng();
        }

        std::mt19937 thread_rng(thread_seed);
        double thread_integral = 0.0;

        // Calculating integral sum.
        #pragma omp for reduction(+:global_integral)
        for (int k = 0; k < samples; k++) {
            double randX = generateRandomNumber(xmin, xmax, thread_rng);
            thread_integral += f_val(randX);
        }

        // Calculating mean.
        thread_integral *= (xmax - xmin) / samples;

        #pragma omp critical
        {
            std::cout << "Integral: " << thread_integral << std::endl;
        }

        #pragma omp atomic
        global_integral += thread_integral;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    int num_threads;
    #pragma omp parallel
    {
        #pragma omp single
        {
            num_threads = omp_get_num_threads();
        }
    }

    std::cout << "Final Integral: " << global_integral/num_threads << std::endl;
    std::cout << "Threads used:" << num_threads << std::endl;
    std::cout << "Total Samples: " << samples << std::endl;
    std::cout << "Elapsed time: " << elapsed.count() << "s" << std::endl;
    return 0;
}