#include <eigen3/Eigen/Sparse>
#include <iostream>
#include <eigen3/unsupported/Eigen/SparseExtra>

using namespace Eigen;

int main(){
  SparseMatrix<double> A;
  loadMarket(A, "bcsstk11.mtx");
  int n = A.cols();
  VectorXd b(n), x(n);
  ConjugateGradient<SparseMatrix<double>, Lower|Upper> cg;
  cg.compute(A);
  x = cg.solveWithGuess(b, VectorXd::Constant(n, 1.0));
  std::cout << "#iterations:     " << cg.iterations() << std::endl;
  std::cout << "estimated error: " << cg.error()      << std::endl;
  std::cout << A.rows() << std::endl;
}
