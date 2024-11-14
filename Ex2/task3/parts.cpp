#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

//estimating first partial (a,b,c,d,N -> est matrix); x = [a,b], y = [c,d]
double firstpartial(double a, double b, double c,double d, int N){
	double estval;
	
	
	return estval;
}

//estimating second partial (a,b,c,d,N -> est matrix); x = [a,b], y = [c,d]
double secondpartial(double a, double b, double c,double d, int N){
	double estval;
	
	
	return estval;
}

//system matrix generator
vector<vector<double>> output(n, vector<double(n,0));

//source func vector generator (xsrc,ysrc,xvec, yvec, dev -> vector)
vector<vector<double>> sourcevec(int dev, int xsrc, int ysrc, vector<double> xvec, vector<double> yvec){
	double pi = 3.14159265358979323846
	int xn = xvec.size();
	int yn = yvec.size();
	vector<vector<double>> output(xn, vector<double(yn,0));
	
	for (int i = 0; i < xn; i++){
		for (int j = 0; j < yn; j++){
			output[i][j] = 1 / (2 * pi * pow(dev,2)) * exp(-(pow((xvec[i] - xsrc),2) + pow(yvec[j] - ysrc,2)) / (2 * pow(dev,2))
		}
	}
	
	return output;
}

// jacobi method of solving PDEs (matrix, vector -> vector)
// followed pseudocode from https://en.wikipedia.org/wiki/Jacobi_method
vector<double> jacobi(const vector<vector<double>>& Amat, vector<double> source, vector<double> xvec, double yval, int N, int maxiter){
	int h = 1 / (N-1)
	int iter = 0;
	vector<double> outprev(N);
	vector<double> outnew(N);
	
	while (iter < maxiter){
		for (int i = 0; i < N; i++){
			int sigma = 0;
			for (int j = 0; j < N; j++){
				if (j != i){
					sigma = sigma + Amat[i][j] * outprev[j];
				}
			}
			outnew[i] = (source[i] - sigma) / Amat[i][i];
		}
	//---------
		outprev = outnew;
		iter ++;
	}
	
	return outnew; //print iteration number somehow

}
//2norm calc

//infnorm calc

int main(){
	int N = 10
	
    auto start = high_resolution_clock::now();
    //insert timed function here
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "1-norm for 1000 x 1000 matrix = " << final <<endl;
    std::cout << duration.count() <<endl;
    
    return 0;
}

void main(){}

