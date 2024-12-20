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
  std::vector<Test> Tests = {
      100,       200,       300,       400,       500,       600,       700,       800,       900,
      1'000,     2'000,     3'000,     4'000,     5'000,     6'000,     7'000,     8'000,     9'000, 
      10'000,    20'000,    30'000,    40'000,    50'000,    60'000,    70'000,    80'000,    90'000,
      100'000,   200'000,   300'000,   400'000,   500'000,   600'000,   700'000,   800'000,   900'000, 
      1'000'000, 2'000'000, 3'000'000, 4'000'000, 5'000'000, 6'000'000, 7'000'000, 8'000'000, 9'000'000, 
      10'000'000,20'000'000,30'000'000,40'000'000,50'000'000};

  for (auto &test : Tests) {
    Vector a(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector b(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector c(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector d(test.N, 1.0);  // 200/300/.../10000000 ones
    int N = 40;
    for (int n = 0; n != N; ++n) // averaged triad runtime
      test.runtime += timeit::run(triad, a, b, c, d);
    test.runtime /= N;
  }
  
  for (auto &test : Tests) {
    test.print2(); // print values only
  }
  
  return 0;
}
