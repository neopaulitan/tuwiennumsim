#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

//source func vector generator (xsrc,ysrc,xvec, yvec, dev -> vector)
//removes pts @ x = 0 & x = 1
vector<double> sourcevec(double dev, double xsrc, double ysrc, double xstart, double ystart, int N){
	double pi = 3.14159265358979323846;
	double h = 1.0/(N-1);
	int adj = N - 2;
	vector<double> output(adj * adj, 0.0);
	
	xstart += h;
	ystart += h;
	
	for (int i = 0; i < adj; i++){
		//row num
		double ypos = ystart + i * h;
		for (int j = 0; j < adj; j++){
			//col num
			double xpos = xstart + j * h;
			int pos = i + j * N;
			output[pos] = 1 / (2 * pi * pow(dev,2)) * 
						exp(-(pow((xpos) - xsrc,2) + 
								pow((ypos) - ysrc,2)) / 
								(2 * pow(dev,2)));
		}
	}
	
	return output;
}

//system matrix generator
//inspired from https://aquaulb.github.io/book_solving_pde_mooc/solving_pde_mooc/notebooks/05_IterativeMethods/05_01_Iteration_and_2D.html
vector<vector<double>> Amatgen(double n, double dirichlet, double neumann){
	double h = 1.0 / (n-1);
	double cx = 1.0 / pow(h,2);
	double cy = 1.0 / pow(h,2);
	double c1 = -2*cx-2*cy;
	int adj = n-2;
	//cout << n << adj << endl;
	int matsize = adj * adj;
	
	vector<vector<double>> output(matsize, vector<double>(matsize, 0.0));

	//row major ordering requires going through every x value at a y value
	//careful with orientation
	for (int j = 0; j <adj; j++){
		for(int i = 0; i<adj; i++){
			int pos = i + j * adj;
		
			//if (i==0||i==n-1||j==0||j==n-1){
			//	output[pos][pos] = dirichlet;
			//	continue;
			//}
			
			output[pos][pos] = c1;
			
			if (j>0){
				output[pos][pos-adj] = cy;
			}
			if (j<adj-1){
				output[pos][pos+adj] = cy;
			}
			if (i>0){		
				output[pos][pos-1] = cx;
			}
			if (i<adj-1){
				output[pos][pos+1] = cx;
			}
		}
	}
	
	if(neumann!=0.0){
		for (int i = 0; i<adj; i++){
			int bot = i;
			int top = i + (n-3) * n;
			output[bot][bot] += neumann * h;
			output[top][top] -= neumann * h;
		}
	}
	
	return output;
}

// jacobi method of solving PDEs (matrix, vector -> vector)
// followed pseudocode from https://en.wikipedia.org/wiki/Jacobi_method
vector<double> jacobi(const vector<vector<double>>& Amat, vector<double> source, int N, int maxiter){	
	int Asize = Amat[0].size();
	//cout << Asize << endl;
	int soursize = source.size();
	//cout << soursize << endl;
	cout<<"passin1"<<endl;
	
	vector<double> outprev(Asize, 0.0);
	vector<double> outnew(Asize, 0.0);
	
	cout<<"passin2"<<endl;
	
	if (Asize != soursize){
		cout<<"matrix size = " <<Asize<< " vector size = " << soursize <<endl;
		return outnew;
	}
	cout<<"passin3"<<endl;
	
	int iter = 0;
	while (iter < maxiter){
		for (int i = 0; i < Asize; i++){
			double sigma = 0.0;
			for (int j = 0; j < Asize; j++){
				if (j != i){
					sigma += Amat[i][j] * outprev[j];
				}
			}
			outnew[i] = (source[i] - sigma) / Amat[i][i];
			cout << Amat[i][i] << endl;
		}
	//---------
		outprev = outnew;
		iter ++;
	}

	return outnew; 

}

//2norm calc
double twonormcalc(vector<double> input){
	int n = input.size();
	double twonorm = 0.0;
	for (int i = 0; i < n; i++){
		twonorm += pow(input[i],2);
	}
	twonorm = pow(twonorm, 1/2);
	return twonorm;
}

//infnorm calc
double infnorm(vector<double> input){
	int n = input.size();
	double twonorm = 0.0;
	for (int i = 0; i < n; i++){
		twonorm = max(twonorm, abs(input[i]));
	}
	return twonorm;
}

//diag dominant check
void diagdomchecker(vector<vector<double>> input){
	string success = "the provided matrix is diagonally dominant";
	string fail = "the provided matrix is not diagonally dominant";
	
	int ny = input.size();
	int nx = input[0].size();
	if (nx != ny){
		cout << fail << endl;
		return;
	}
	
	for (int i = 0; i < ny; i++){
		double compar = abs(input[i][i]);
		double sum = 0.0;
		for (int j=0; j<ny; j++){
			if (j!=i){	
				sum += abs(input[i][j]);
			}
		}
		if (compar < sum){
			cout << fail << endl;
			return;
		}
	}
	cout << success << endl;
	return;
}

vector<double> xvecgen(int n, double xstart){
	vector<double>output(n * n,0.0);
	double h = 1.0 / (n-1);
	
	for (int i = 0; i < n; i++){
		for (int j = 0; j<n; j++){
			int pos = j + i*n;
			output[pos] = xstart + j * h;
		}
	}
	return output;
}

void solntocsv(const vector<vector<double>>& input, string& filename){
	filename +=".csv";
	ofstream csv(filename);
	
	for (int i = 0; i < input.size(); i++){
		for (int j = 0; j < input[0].size(); j++){ 
			csv << input[i][j];
			if (j != input[0].size() - 1){
				csv << ",";
			}
		}
		csv << endl;
	}
	csv.close();
	cout << "soln written to " << filename << endl;
}

vector<double> matvecmult(vector<vector<double>> matrix, vector<double> vect){
	int n = vect.size();
	vector<double>output(n, 0.0);
	for (int i = 0; i < n; i++){
		for (int j = 0; j<n; j++){
			output[i] += matrix[i][j] * vect[j];
		}
	}
	
	return output;
}

//general code flow: generate active grid matrix in row major order -> generate source -> apply jacobi method and time it ->soln is packed with xaxis points and includes boundary points in a Nx2 csv-> find norms of residuals using matrix, source and solution

int main(){
	int N = 10;
	int maxiter = 10000;
	double xsrc = 0.0;
	double ysrc = 0.0;
	double dev	= 0.5;
	double xstart = 0.0;
	double ystart = 0.0;
	
	double dirichlet = 0.0;
	double neumann = 0.0;
	string filename = "output";
	int matsize = (N-2)*(N-2);
	//cout<<"pass1"<<endl;
	
	vector<vector<double>> A_matrix(matsize, vector<double>(matsize, 0.0));
	A_matrix = Amatgen(N, dirichlet, neumann);
	diagdomchecker(A_matrix);
	//cout<<"pass2"<<endl;
	
	vector<double> sourcevector(matsize, 0.0);
	sourcevector = sourcevec(dev, xsrc, ysrc, xstart, ystart, N);
	//cout << "check sourcevec:" << sourcevector[20] << endl;
	
	vector<double> soln(matsize, 0.0);
	
    auto start = high_resolution_clock::now();
    soln = jacobi(A_matrix, sourcevector, N, maxiter);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "jacobi process' time taken: " << duration.count() << "E-6 s" <<endl;
    std::cout << duration.count() <<endl;
    
    vector<double>xrowvec(N*N, 0.0);
   	xrowvec = xvecgen(N,xstart);
   	cout<<"pass3"<<endl;
   	vector<vector<double>>pack(N*N, vector<double>(2, 0.0));
   	
   	int row = 0;
   	while (row < N){
   		for (int i = 0; i < N; i++){
   			int pos = row * N + i;
   			if (i == 0 || i == N-1){
   				pack[pos][1] = 0; //applying dirichlet cond.
   			}
   			else{
   				pack[pos][1] = soln[i];
   			} 
   		}
   		row +=1;
   	}
   	
   	solntocsv(pack, filename);
    cout<<"pass4"<<endl;
    
    vector<double>resid(N * N, 0.0);
    resid = matvecmult(A_matrix, soln);
    
    for (int i = 0; i < N * N; i++){
    	resid[i] -= sourcevector[i];
    }
    
    double twonormval;
    twonormval = twonormcalc(resid);
    cout << "2-norm value = " << twonormval << endl;
    
	double infnormval;
	infnormval = infnorm(resid);
	cout << "infinite-norm value = " << infnormval << endl;
    
    return 0;
}

