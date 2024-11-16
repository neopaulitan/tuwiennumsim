// sudo apt-get install build-essential libeigen3-dev libopenblas-dev
// g++ main.cpp -std=c++17 -O3 -lopenblas -DEIGEN_DONT_PARALLELIZE -march=native
// -ffast-math -o mmm
// ./mmm CUSTOM 1024
// ./mmm EIGEN 1024
// export OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 && ./mmm BLAS 1024

#include <cassert>
#include <cblas.h>
#include <chrono>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace program_options {
std::tuple<std::string, std::size_t> parse(int argc, char *argv[]) {
  if (argc != 3)
    throw std::runtime_error("unexpected number of arguments");
  auto mode = std::string(argv[1]);
  const std::vector<std::string> modes = {"CUSTOM", "BLAS", "EIGEN"};
  if (std::find(modes.begin(), modes.end(), mode) == modes.end())
    throw std::runtime_error(
        "unsupported mode - Choose either 'CUSTOM', 'BLAS', or 'Eigen'");
  std::size_t size;
  if (std::sscanf(argv[2], "%zu", &size) != 1 && size != 0)
    throw std::runtime_error("invalid size");
  std::printf("arguments: impl=%s size=%zu\n", mode.data(), size);
  return {mode, size};
}
} // namespace program_options

std::chrono::duration<double> MMM_custom(int N) {
  // prepare Matricies
  int M[N][N];
  int MT[N][N];
  int MMT[N][N] = {}; // this is the product of M & MT
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      M[i][j] = i + j * N;
      MT[i][j] = j + i * N;
    }
  }
  // perform actual MMM
  auto start_time = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        MMT[i][j] += M[k][i] * MT[j][k];
      }
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_time = end_time - start_time;
  // perform symmetry check
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      assert(MMT[i][j] == MMT[j][i]);
      // std::cout << "MMT[" << i << "][ " << j <<"] = " << MMT[i][j] <<
      // std::endl;
    }
  }
  return elapsed_time;
}

std::chrono::duration<double> MMM_OpenBLAS(int N) {
  // prepare the matricies
  const double alpha = 1.0;
  const double beta = 0.0;
  std::vector<double> M;
  for (int i = 0; i < N * N; i++) {
    M.push_back(i);
  }
  std::vector<double> MMT(N * N, 0.0);
  // perform the Matrix-Matrix Multiplication
  auto start_time = std::chrono::high_resolution_clock::now();
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, N, N, N, alpha, M.data(),
              N, M.data(), N, beta, MMT.data(), N);
  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_time = end_time - start_time;
  // perform symmetry check
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      assert(MMT[i + j * N] == MMT[j + i * N]);
    }
  }
  /*
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cout << MMT[i * N + j] << " ";
      }
     std::cout << "\n";
     }
  */
  return elapsed_time;
}

std::chrono::duration<double> MMM_Eigen(int N) {
  // prepare the matricies
  Eigen::MatrixXd M(N, N);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      M(i, j) = j + i * N;
    }
  }
  Eigen::MatrixXd MT = M.transpose();
  // perform the Matrix-Matrix Multiplication
  auto start_time = std::chrono::high_resolution_clock::now();
  Eigen::MatrixXd MMT = M * MT;
  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_time = end_time - start_time;
  // perform symmetry check
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      assert(MMT(i, j) == MMT(j, i));
      // std::cout << "MMT(" << i << ", " << j <<") = " << MMT(i,j) <<
      // std::endl;
    }
  }
  return elapsed_time;
}

int main(int argc, char *argv[]) try {
  auto [mode, size] = program_options::parse(argc, argv);
  std::size_t N = size;
  std::chrono::duration<double> elapsed_time;
  if (mode == "CUSTOM") {
   elapsed_time = MMM_custom(N);
  } else if (mode == "EIGEN") {
   elapsed_time = MMM_Eigen(N);
  } else if (mode == "BLAS") {
   elapsed_time = MMM_OpenBLAS(N);
  }
  std::cout << elapsed_time.count() << std::endl;
  return EXIT_SUCCESS;
} catch (std::exception &e) {
  std::cout << e.what() << std::endl;
  return EXIT_FAILURE;
}
