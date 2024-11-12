// sudo apt-get install build-essential libeigen3-dev libopenblas-dev
// g++ main.cpp -std=c++17 -O3 -lopenblas -DEIGEN_DONT_PARALLELIZE -march=native -ffast-math -o mmm
// ./mmm CUSTOM 1024
// ./mmm EIGEN 1024
// export OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 && ./mmm BLAS 1024

#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <cblas.h>
#include <eigen3/Eigen/Dense>

namespace program_options {

std::tuple<std::string, std::size_t> parse(int argc, char *argv[]) {
  if (argc != 3)
    throw std::runtime_error("unexpected number of arguments");
  auto mode = std::string(argv[1]);
  const std::vector<std::string> modes = {"CUSTOM", "BLAS", "EIGEN"};
  if (std::find(modes.begin(), modes.end(), mode) == modes.end())
    throw std::runtime_error("unsupported mode");
  std::size_t size;
  if (std::sscanf(argv[2], "%zu", &size) != 1 && size != 0)
    throw std::runtime_error("invalid size");
  std::printf("arguments: impl=%s size=%zu\n", mode.data(), size);
  return {mode, size};
}

} // namespace program_options

int main(int argc, char *argv[]) try {

  auto [mode, size] = program_options::parse(argc, argv);

  std::size_t N = size;
  std::string impl = mode;

  return EXIT_SUCCESS;
} catch (std::exception &e) {
  std::cout << e.what() << std::endl;
  return EXIT_FAILURE;
}
