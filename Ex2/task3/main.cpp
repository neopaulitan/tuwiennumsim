// g++ main.cpp -std=c++17 -O3 -march=native -ffast-math -o solver
// ./solver without_source 10 10000 0.0 1.0
// ./solver with_source 50 100000 0.0 0.0 0.5 0.5 0.1

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

namespace program_options {

struct Options {
  std::string name;
  size_t N;
  size_t iters;
  double fix_west;
  double fix_east;
  bool has_source;
  double source_x;
  double source_y;
  double source_sigma;
  void print() const {
    std::printf("name: %s\n", name.c_str());
    std::printf("N: %zu\n", N);
    std::printf("iters: %zu\n", iters);
    std::printf("fix_west: %lf\n", fix_west);
    std::printf("fix_east: %lf\n", fix_east);
    std::printf("has_source: %s\n", has_source ? "true" : "false");
    std::printf("source_x: %lf\n", source_x);
    std::printf("source_y: %lf\n", source_y);
    std::printf("source_sigma: %lf\n", source_sigma);
  }
};

auto parse(int argc, char *argv[]) {
  if (argc != 9 && argc != 6)
    throw std::runtime_error("unexpected number of arguments");
  Options opts;
  opts.name = argv[1];
  if (std::sscanf(argv[2], "%zu", &opts.N) != 1 && opts.N >= 2)
    throw std::runtime_error("invalid parameter for N");
  if (std::sscanf(argv[3], "%zu", &opts.iters) != 1 && opts.iters != 0)
    throw std::runtime_error("invalid parameter for iters");
  if (std::sscanf(argv[4], "%lf", &opts.fix_west) != 1)
    throw std::runtime_error("invalid value for fix_west");
  if (std::sscanf(argv[5], "%lf", &opts.fix_east) != 1)
    throw std::runtime_error("invalid value for fix_east");
  if (argc == 6) {
    opts.has_source = false;
    opts.source_x = NAN;
    opts.source_y = NAN;
    opts.source_sigma = NAN;
    return opts;
  }
  if (std::sscanf(argv[6], "%lf", &opts.source_x) != 1)
    throw std::runtime_error("invalid value for source_x");
  if (std::sscanf(argv[7], "%lf", &opts.source_y) != 1)
    throw std::runtime_error("invalid value for source_y");
  if (std::sscanf(argv[8], "%lf", &opts.source_sigma) != 1)
    throw std::runtime_error("invalid value for source_sigma");
  opts.has_source = true;
  return opts;
}

} // namespace program_options

struct mesh{
    int N; // width && height
    int NN; // mesh size
    int matsize; // size of non-boundary part (i.e.# unknowns)=(N-2)*(N-2)
    double h; // 1.0/N
};

// LHS matrix A
void Amatgen(mesh Mesh, double **A){
	double cx=-1.0/std::pow(Mesh.h, 2);
	double cy=-1.0/std::pow(Mesh.h, 2);
	double c1=2*cx+2*cy;
	for(int j=0; j<Mesh.N-2; j++){
		for(int i=0; i<Mesh.N-2; i++){
			int idx=i+j*(Mesh.N-2);
			A[idx][idx]=c1;
			if(j>0){
				A[idx][idx-(Mesh.N-2)]=cy;
			}
			if(j<Mesh.N-3){
				A[idx][idx+(Mesh.N-2)]=cy;
			}
			if(i>0){
				A[idx][idx-1]=cx;
			}
			if(i<Mesh.N-3){
				A[idx][idx+1]=cx;
			}
		}
	}
	return;
}

// RHS vector b
void  sourcevec(mesh Mesh, double *b, double dev, double xsrc, double ysrc, double BC_dirichlet_W, double BC_dirichlet_E, double BC_neumann=0.0){
	// basic source term (i,j only for blue points)
	for (int j=0; j<Mesh.N-2; j++){
		double ypos=Mesh.h+j*Mesh.h;
    for (int i=0; i<Mesh.N-2; i++){
      int idx=i+j*(Mesh.N-2);
      double xpos=Mesh.h+i*Mesh.h;
			b[idx]=1.0/(2*M_PI*std::pow(dev, 2)) * exp(-(std::pow(xpos-xsrc, 2)+std::pow(ypos-ysrc, 2))/(2*std::pow(dev, 2)));
		}
	}
  // applying boundary conditions
  // bottom Neumann cells: j=0
  for(int i=0; i<Mesh.N-2; i++){
    int idx=i+0*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_neumann; // b+1/h^2 * u_S = b+0
  }
  // top Neumann cells: j=N-3
  for(int i=0; i<Mesh.N-2; i++){
    int idx=i+(Mesh.N-3)*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_neumann; // b+1/h^2 * u_N = b+0
  }
  // left Dirichlet cells: i=0
  for(int j=0; j<Mesh.N-2; j++){
    int idx=0+j*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_dirichlet_W; // b+1/h^2 * u_W
  }
  // right Dirichlet cells: i=N-3
  for(int j=0; j<Mesh.N-2; j++){
    int idx=(Mesh.N-3)+j*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_dirichlet_E; // b+1/h^2 * u_E
  }
	return;
}

//diag dominant check
void diagdomchecker(mesh Mesh, double **A){
	string success = "the provided matrix is diagonally dominant";
	string fail = "the provided matrix is not diagonally dominant";
	
	int ny = Mesh.matsize;
	int nx = Mesh.matsize;
	if (nx != ny){
		cout << fail << endl;
		return;
	}
	
	for (int i = 0; i < ny; i++){
		double compar = std::abs(A[i][i]);
		double sum = 0.0;
		for (int j=0; j<ny; j++){
			if (j!=i){	
				sum += std::abs(A[i][j]);
			}
		}
    // std::cout<<compar<<"VS"<<sum<<std::endl;
		if (compar < sum){
			cout << fail << endl;
			return;
		}
	}
	cout << success << endl;
	return;
}

// jacobi method of solving PDEs (matrix, vector -> vector)
// followed pseudocode from https://en.wikipedia.org/wiki/Jacobi_method
void jacobi(mesh Mesh, double **A, double *u, double *b, int maxiter){	
	int Asize = Mesh.matsize;
	int bsize = Mesh.matsize;
	int iter=0;
	while(iter<maxiter){
		for(int i=0; i<Asize; i++){
			double sum=0.0;
			for(int j=0; j<Asize; j++){
				if(j!=i){
					sum+=A[i][j]*u[j];
				}
			}
			u[i]=(b[i]-sum)/A[i][i];
      std::cout<<i<<"/"<<Mesh.matsize<<std::endl;
		}
		iter++;
	}
  return;
}

int main(int argc, char *argv[]) try{
  auto opts = program_options::parse(argc, argv);
  opts.print();
  // write mesh values into csv
  auto write = [ N = opts.N, name = opts.name ](const auto &x) -> auto {
    std::ofstream csv;
    csv.open(name + ".csv");
    for (size_t j = 0; j < N; ++j) {
      for (size_t i = 0; i < N - 1; ++i) {
        csv << x[i + j * N] << " ";
      }
      csv << x[(N - 1) + j * N];
      csv << "\n";
    }
    csv.close();
  };

  // initialize mesh
  mesh Mesh;
  Mesh.N=opts.N;
  Mesh.h=1.0/(opts.N-1);
  Mesh.NN=opts.N*opts.N;
  Mesh.matsize=(opts.N-2)*(opts.N-2);

  // 3.1.1: initialize A, b
  double** A=new double*[Mesh.matsize];
  for(int i=0; i<Mesh.matsize; i++){ A[i]=new double[Mesh.matsize]; }
  for(int i=0; i<Mesh.matsize; ++i){
      for(int j=0; j<Mesh.matsize; ++j){
          A[i][j] = 0;
      }
  }
  Amatgen(Mesh, A);
  
  double* b=new double[Mesh.matsize];
  for(int i=0; i<Mesh.matsize; i++){ b[i]=0; }
  if(argc==6){
    sourcevec(Mesh, b, 1, 0.5, 0.5, opts.fix_west, opts.fix_east);
  }
  else if(argc==9){
    sourcevec(Mesh, b, opts.source_sigma, opts.source_x, opts.source_y, opts.fix_west, opts.fix_east);
  }

  // 3.1.2: illustrate A
  std::cout<<"writing A into csv"<<std::endl;
  std::ofstream Aout("./A.csv");
  for(int j=0; j<Mesh.matsize; j++){
    for(int i=0; i<Mesh.matsize; i++){
      Aout<<std::setw(5)<<A[i][j]<<",";
    }
    Aout<<std::endl;
  }
  Aout.close();
  // 3.1.2(1): print b by the way
  std::ofstream bout("./b.csv");
  for(int i=0; i<Mesh.matsize; i++){
    bout<<b[i]<<std::endl;
  }
  bout.close();
  // 3.1.3: verify diagonally dominant
	diagdomchecker(Mesh, A);

  // 3.2
  double *u=new double[Mesh.matsize];
  for(int i=0; i<Mesh.matsize; i++){ u[i]=0; }
  auto start=high_resolution_clock::now();
  jacobi(Mesh, A, u, b, opts.iters);
  auto stop=high_resolution_clock::now();
  std::chrono::duration<double> elapsed_time = stop - start;
  std::cout<<"runtime: "<<elapsed_time.count()<<"s"<<std::endl;
  write(u); // write out the mesh

  return EXIT_SUCCESS;
} catch (std::exception &e) {
  std::cout << e.what() << std::endl;
  return EXIT_FAILURE;
}
// g++ main.cpp -std=c++17 -O3 -march=native -ffast-math -o solver
// ./solver without_source 10 10000 0.0 1.0
// ./solver with_source 50 100000 0.0 0.0 0.5 0.5 0.1

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

namespace program_options {

struct Options {
  std::string name;
  size_t N;
  size_t iters;
  double fix_west;
  double fix_east;
  bool has_source;
  double source_x;
  double source_y;
  double source_sigma;
  void print() const {
    std::printf("name: %s\n", name.c_str());
    std::printf("N: %zu\n", N);
    std::printf("iters: %zu\n", iters);
    std::printf("fix_west: %lf\n", fix_west);
    std::printf("fix_east: %lf\n", fix_east);
    std::printf("has_source: %s\n", has_source ? "true" : "false");
    std::printf("source_x: %lf\n", source_x);
    std::printf("source_y: %lf\n", source_y);
    std::printf("source_sigma: %lf\n", source_sigma);
  }
};

auto parse(int argc, char *argv[]) {
  if (argc != 9 && argc != 6)
    throw std::runtime_error("unexpected number of arguments");
  Options opts;
  opts.name = argv[1];
  if (std::sscanf(argv[2], "%zu", &opts.N) != 1 && opts.N >= 2)
    throw std::runtime_error("invalid parameter for N");
  if (std::sscanf(argv[3], "%zu", &opts.iters) != 1 && opts.iters != 0)
    throw std::runtime_error("invalid parameter for iters");
  if (std::sscanf(argv[4], "%lf", &opts.fix_west) != 1)
    throw std::runtime_error("invalid value for fix_west");
  if (std::sscanf(argv[5], "%lf", &opts.fix_east) != 1)
    throw std::runtime_error("invalid value for fix_east");
  if (argc == 6) {
    opts.has_source = false;
    opts.source_x = NAN;
    opts.source_y = NAN;
    opts.source_sigma = NAN;
    return opts;
  }
  if (std::sscanf(argv[6], "%lf", &opts.source_x) != 1)
    throw std::runtime_error("invalid value for source_x");
  if (std::sscanf(argv[7], "%lf", &opts.source_y) != 1)
    throw std::runtime_error("invalid value for source_y");
  if (std::sscanf(argv[8], "%lf", &opts.source_sigma) != 1)
    throw std::runtime_error("invalid value for source_sigma");
  opts.has_source = true;
  return opts;
}

} // namespace program_options

struct mesh{
    int N; // width && height
    int NN; // mesh size
    int matsize; // size of non-boundary part (i.e.# unknowns)=(N-2)*(N-2)
    double h; // 1.0/N
};

// LHS matrix A
void Amatgen(mesh Mesh, double **A){
	double cx=-1.0/std::pow(Mesh.h, 2);
	double cy=-1.0/std::pow(Mesh.h, 2);
	double c1=2*cx+2*cy;
	for(int j=0; j<Mesh.N-2; j++){
		for(int i=0; i<Mesh.N-2; i++){
			int idx=i+j*(Mesh.N-2);
			A[idx][idx]=c1;
			if(j>0){
				A[idx][idx-(Mesh.N-2)]=cy;
			}
			if(j<Mesh.N-3){
				A[idx][idx+(Mesh.N-2)]=cy;
			}
			if(i>0){
				A[idx][idx-1]=cx;
			}
			if(i<Mesh.N-3){
				A[idx][idx+1]=cx;
			}
		}
	}
	return;
}

// RHS vector b
void  sourcevec(mesh Mesh, double *b, double dev, double xsrc, double ysrc, double BC_dirichlet_W, double BC_dirichlet_E, double BC_neumann=0.0){
	// basic source term (i,j only for blue points)
	for (int j=0; j<Mesh.N-2; j++){
		double ypos=Mesh.h+j*Mesh.h;
    for (int i=0; i<Mesh.N-2; i++){
      int idx=i+j*(Mesh.N-2);
      double xpos=Mesh.h+i*Mesh.h;
			b[idx]=1.0/(2*M_PI*std::pow(dev, 2)) * exp(-(std::pow(xpos-xsrc, 2)+std::pow(ypos-ysrc, 2))/(2*std::pow(dev, 2)));
		}
	}
  // applying boundary conditions
  // bottom Neumann cells: j=0
  for(int i=0; i<Mesh.N-2; i++){
    int idx=i+0*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_neumann; // b+1/h^2 * u_S = b+0
  }
  // top Neumann cells: j=N-3
  for(int i=0; i<Mesh.N-2; i++){
    int idx=i+(Mesh.N-3)*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_neumann; // b+1/h^2 * u_N = b+0
  }
  // left Dirichlet cells: i=0
  for(int j=0; j<Mesh.N-2; j++){
    int idx=0+j*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_dirichlet_W; // b+1/h^2 * u_W
  }
  // right Dirichlet cells: i=N-3
  for(int j=0; j<Mesh.N-2; j++){
    int idx=(Mesh.N-3)+j*(Mesh.N-2);
    b[idx]+=1.0/std::pow(Mesh.h, 2)*BC_dirichlet_E; // b+1/h^2 * u_E
  }
	return;
}

//diag dominant check
void diagdomchecker(mesh Mesh, double **A){
	string success = "the provided matrix is diagonally dominant";
	string fail = "the provided matrix is not diagonally dominant";
	
	int ny = Mesh.matsize;
	int nx = Mesh.matsize;
	if (nx != ny){
		cout << fail << endl;
		return;
	}
	
	for (int i = 0; i < ny; i++){
		double compar = std::abs(A[i][i]);
		double sum = 0.0;
		for (int j=0; j<ny; j++){
			if (j!=i){	
				sum += std::abs(A[i][j]);
			}
		}
    // std::cout<<compar<<"VS"<<sum<<std::endl;
		if (compar < sum){
			cout << fail << endl;
			return;
		}
	}
	cout << success << endl;
	return;
}

// jacobi method of solving PDEs (matrix, vector -> vector)
// followed pseudocode from https://en.wikipedia.org/wiki/Jacobi_method
void jacobi(mesh Mesh, double **A, double *u, double *b, int maxiter){	
	int Asize = Mesh.matsize;
	int bsize = Mesh.matsize;
	int iter=0;
	while(iter<maxiter){
		for(int i=0; i<Asize; i++){
			double sum=0.0;
			for(int j=0; j<Asize; j++){
				if(j!=i){
					sum+=A[i][j]*u[j];
				}
			}
			u[i]=(b[i]-sum)/A[i][i];
      std::cout<<i<<"/"<<Mesh.matsize<<std::endl;
		}
		iter++;
	}
  return;
}

int main(int argc, char *argv[]) try{
  auto opts = program_options::parse(argc, argv);
  opts.print();
  // write mesh values into csv
  auto write = [ N = opts.N, name = opts.name ](const auto &x) -> auto {
    std::ofstream csv;
    csv.open(name + ".csv");
    for (size_t j = 0; j < N; ++j) {
      for (size_t i = 0; i < N - 1; ++i) {
        csv << x[i + j * N] << " ";
      }
      csv << x[(N - 1) + j * N];
      csv << "\n";
    }
    csv.close();
  };

  // initialize mesh
  mesh Mesh;
  Mesh.N=opts.N;
  Mesh.h=1.0/(opts.N-1);
  Mesh.NN=opts.N*opts.N;
  Mesh.matsize=(opts.N-2)*(opts.N-2);

  // 3.1.1: initialize A, b
  double** A=new double*[Mesh.matsize];
  for(int i=0; i<Mesh.matsize; i++){ A[i]=new double[Mesh.matsize]; }
  for(int i=0; i<Mesh.matsize; ++i){
      for(int j=0; j<Mesh.matsize; ++j){
          A[i][j] = 0;
      }
  }
  Amatgen(Mesh, A);
  
  double* b=new double[Mesh.matsize];
  for(int i=0; i<Mesh.matsize; i++){ b[i]=0; }
  if(argc==6){
    sourcevec(Mesh, b, 1, 0.5, 0.5, opts.fix_west, opts.fix_east);
  }
  else if(argc==9){
    sourcevec(Mesh, b, opts.source_sigma, opts.source_x, opts.source_y, opts.fix_west, opts.fix_east);
  }

  // 3.1.2: illustrate A
  std::cout<<"writing A into csv"<<std::endl;
  std::ofstream Aout("./A.csv");
  for(int j=0; j<Mesh.matsize; j++){
    for(int i=0; i<Mesh.matsize; i++){
      Aout<<std::setw(5)<<A[i][j]<<",";
    }
    Aout<<std::endl;
  }
  Aout.close();
  // 3.1.2(1): print b by the way
  std::ofstream bout("./b.csv");
  for(int i=0; i<Mesh.matsize; i++){
    bout<<b[i]<<std::endl;
  }
  bout.close();
  // 3.1.3: verify diagonally dominant
	diagdomchecker(Mesh, A);

  // 3.2
  double *u=new double[Mesh.matsize];
  for(int i=0; i<Mesh.matsize; i++){ u[i]=0; }
  auto start=high_resolution_clock::now();
  jacobi(Mesh, A, u, b, opts.iters);
  auto stop=high_resolution_clock::now();
  std::chrono::duration<double> elapsed_time = stop - start;
  std::cout<<"runtime: "<<elapsed_time.count()<<"s"<<std::endl;
  write(u); // write out the mesh

  return EXIT_SUCCESS;
} catch (std::exception &e) {
  std::cout << e.what() << std::endl;
  return EXIT_FAILURE;
}
