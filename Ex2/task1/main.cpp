// g++ -std=c++17 -O3 -march=native main.cpp -o triad
// taskset -c 0 ./triad > triad_data.txt

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

namespace timeit {

template <typename CALLABLE, typename... ARGS>
double run(CALLABLE f, ARGS &&... args) {
  using Clock = std::chrono::steady_clock;
  using Duration = std::chrono::duration<double>;
  auto start = Clock::now();
  f(std::forward<ARGS>(args)...);
  auto stop = Clock::now();
  return Duration(stop - start).count();
}

} // namespace timeit

using Vector = std::vector<double>;

void triad(Vector &a, const Vector &b, const Vector &c, const Vector &d) {
  auto aptr = a.data();
  const auto bptr = b.data(); // ptr to the 1st/(1+i)th element of vector
  const auto cptr = c.data(); // ptr to the 1st/(1+i)th element of vector
  const auto dptr = d.data(); // ptr to the 1st/(1+i)th element of vector
  for (std::size_t i = 0; i < c.size(); ++i)
    aptr[i] = bptr[i] + cptr[i] * dptr[i];
}

struct Test {
  std::size_t N;      // vector length
  double runtime = 0; // recorded runtime
  Test(std::size_t N) : N(N) {}
  void print() const {
    std::printf("vector length: %zu\n", N);
    std::printf("memory footprint: %f kbytes\n",
                N * 4.0 * sizeof(double) / 1024.0);
    std::printf("total flops performed: %zu\n", N * 2); // FLOPs per a=b+c*d
    std::printf("runtime per sweep: %e\n", runtime);
  }
  void print1() const { std::printf("%zu %zu %e\n", N, 2*N, runtime); } // #iter, #FLOP, runtime
  void print2() const { std::printf("%zu %e\n", N, runtime); }
};

// split line by required deliminator
std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        if(item.length()!=0){ result.push_back(item); }
    }
    return result;
}

int main() {
  std::vector<Test> Tests ={2, 4, 8, 16, 32, 64, 128, 200, 256, 300, 400, 512, 600, 700,
    800, 900,1000, 1023, 1024, 1025,1030, 1050, 1100, 1400, 1600, 1800, 2000, 2047, 2048, 2049, 2100
      , 2300, 2500, 3000, 3072, 3100, 3300, 3600, 3900, 4096, 4500, 4800, 5000, 5500,
    6000,6143 ,6144,6145, 6200, 
    6500,  7000, 8000, 8192, 8600,
    9000, 10'000, 10'240, 10500,11000, 12000, 13000, 14000, 15000, 20000, 25000, 
 30000, 35000, 36000, 40000, 50000, 60000, 70000, 80000, 90000, 100000,
  200000, 250000, 300000, 400000, 5000000};
  for (auto &test : Tests) {
    Vector a(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector b(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector c(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector d(test.N, 1.0);  // 200/300/.../10000000 ones
    for (int n = 0; n != 19; ++n)
      timeit::run(triad, a, b, c, d); // warmup
    int N = 40;
    for (int n = 0; n != N; ++n) // averaged triad runtime
      test.runtime += timeit::run(triad, a, b, c, d);
    test.runtime /= N;
  }
  
  for (auto &test : Tests) {
    // test.print(); // print with details
    test.print2(); // print values only
  }
  
  return 0;
}
