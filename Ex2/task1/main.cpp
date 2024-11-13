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
      200,     300,     400,       800,       2'000,     4'000,     6'000,
      8'000,   20'000,  30'000,    40'000,    50'000,    60'000,    100'000,
      200'000, 300'000, 1'000'000, 2'000'000, 5'000'000, 10'000'000};

  // 1: vector length (N) VS FLOPs
  for (auto &test : Tests) {
    Vector a(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector b(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector c(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector d(test.N, 1.0);  // 200/300/.../10000000 ones
    // average triad runtime
    for (int i=0; i!=40; ++i){
      test.runtime+=timeit::run(triad, a, b, c, d);
    }
    test.runtime/=40;
    // FLOPs/s
    double FLOPs=2*test.N/test.runtime;
    std::cout<<"FLOPs: "<<FLOPs<<std::endl;
    // test.print1(); // details
  }
  
  // 2: indicate cache size of my system
  system("rm ./system_info.txt");
  system("lscpu >> ./system_info.txt");
  double L1d, L1i, L2, L3;
  std::ifstream system_info;
  system_info.open("./system_info.txt");
  std::string value;
  while(std::getline(system_info, value)){
      // L1d
      if(value.find("L1d")!=std::string::npos){
        L1d=stod(split(value, ' ')[2]);
      }
      // L1i
      if(value.find("L1i")!=std::string::npos){
        L1i=stod(split(value, ' ')[2]);
      }
      // L2
      if(value.find("L2")!=std::string::npos){
        L2=stod(split(value, ' ')[2]);
      }
      // L3
      if(value.find("L3")!=std::string::npos){
        L3=stod(split(value, ' ')[2]);
      }
  }
  std::cout<<"L1d:"<<L1d<<"KB L1i:"<<L1i<<"KB L2:"<<L2<<"MB L3:"<<L3<<"MB"<<std::endl;
  system_info.close();

  for (auto &test : Tests) {
    Vector a(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector b(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector c(test.N, 1.0);  // 200/300/.../10000000 ones
    Vector d(test.N, 1.0);  // 200/300/.../10000000 ones
    for (int n = 0; n != 19; ++n)
      timeit::run(triad, a, b, c, d); // repeat x19 times of triad(i.e. loop a[i]=b[i]+c[i]*d[i])
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
