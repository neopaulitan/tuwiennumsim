#include <eigen3/Eigen/Sparse>
#include <iostream>
#include <fstream>
#include <eigen3/unsupported/Eigen/SparseExtra>
#include <eigen3/Eigen/SparseCholesky>

using namespace Eigen;


template <typename Preconditioner>
void trackResiduals(const SparseMatrix<double>& A, const VectorXd& b, ConjugateGradient<SparseMatrix<double>, Lower | Upper, Preconditioner>& solver, std::vector<double>& residuals) {
    solver.setTolerance(1e-8);
    solver.setMaxIterations(999);
    solver.compute(A);

    if (solver.info() != Success) {
        std::cerr << "Error!" << std::endl;
        return;
    }

    VectorXd x = VectorXd::Zero(A.rows());
    residuals.clear();
		float r_0 =  (b - A * x).norm();
    for (int k = 0; k < solver.maxIterations(); ++k) {
        x = solver.solveWithGuess(b, x);
        float residual_improvement_factor = solver.error()/ r_0;
        residuals.push_back(residual_improvement_factor);
    		//std::cout << "Iteration " << k << ", residual: " << residuals.back() << std::endl;
    }
}

int main(){
  SparseMatrix<double> A;
  loadMarket(A, "bcsstk11.mtx");


  SparseMatrix<double> A_test(3,3);
  A_test.insert(0, 0) = 4;
  A_test.insert(0, 1) = 1;
  A_test.insert(1, 1) = 3;
  A_test.insert(0, 2) = 2;
  A_test.insert(2, 2) = 5;
  A_test.insert(1, 0) = 1;
  A_test.insert(2, 0) = 2;

  //A = A_test;

  int n = A.rows();
  VectorXd x_star = VectorXd::Ones(n);
  VectorXd b = A * x_star;
  A /= A.norm();
	b /= b.norm();
	SparseMatrix<double> A_symmetric = A.selfadjointView<Lower>();
	A = A_symmetric;
  
  // Residual tracking
  std::vector<double> residual_diag, residual_ic;

  // Solve with Diagonal Preconditioner
  ConjugateGradient<SparseMatrix<double>, Lower | Upper, DiagonalPreconditioner<double>> cg_diag;
  trackResiduals(A, b, cg_diag, residual_diag);

  // Solve with IncompleteCholesky Preconditioner
  ConjugateGradient<SparseMatrix<double>, Lower | Upper, IncompleteCholesky<double>> cg_ic;
  trackResiduals(A, b, cg_ic, residual_ic);
  
  // write residuals to csv file
  std::string filename = "out.csv";
  std::ofstream file(filename);
  file << "k, residual_diag, residual_ic" << std::endl;
  for (int i = 0;i<999;i++){
  	file << i << ", " << residual_diag[i] << ", " << residual_ic[i] << std::endl;
  }
  file.close(); 
  return 0;
}
