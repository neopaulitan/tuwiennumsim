#include <eigen3/Eigen/Core>
#include <iostream>
#include <fstream>

using namespace Eigen;

struct FULL{
    int rows;
    int cols;
    int nonZeros;
    // note that A is a translation from COO into full matrix
    double** A;
};

// (a)
struct CCS{
    int rows;
    int cols;
    int nonZeros;
    std::vector<double> V;
    std::vector<int> IA;
    std::vector<int> JA;
};

// (b)
FULL mtx_reader(std::string file){
    std::ifstream mat;
    mat.open("./"+file);
    std::string line;
    // skip comments
    std::getline(mat, line);
    if(line[0]!='%'){ throw std::invalid_argument("please give me correct data"); } // Header lines start with '%'
    
    // initialize dimensions
    FULL full;
    std::getline(mat, line);
    std::istringstream dimensions(line);
    dimensions >> full.rows >> full.cols >> full.nonZeros;
    full.A = new double*[full.rows];
    for(int i=0; i<full.rows; i++){
        full.A[i] = new double[full.cols];
        for(int j=0; j<full.cols; j++){
            full.A[i][j]=0;
        }
    }
    // matrix values
    while(std::getline(mat, line)){
        int row, col;
        double value;
        std::istringstream values(line);
        values >> row >> col >> value;
        full.A[row-1][col-1]=value;
    }
    return full;
}

CCS FULL_2_L_CCS(FULL full){
    CCS ccs;
    ccs.rows = full.rows;
    ccs.cols = full.cols;
    ccs.nonZeros = full.nonZeros;
    for(int j=0; j<ccs.cols; j++){
        ccs.JA.push_back(ccs.V.size());
        for(int i=j; i<ccs.rows; ++i){
            if(full.A[i][j]!=0){
                ccs.V.push_back(full.A[i][j]);
                ccs.IA.push_back(i);
            }
        }
    }
    ccs.JA.push_back(ccs.V.size());
    return ccs;
}

// (c)(d)
double* MVM(CCS ccs, double* x){ // matrix vector multiply
    double* y = new double[ccs.rows];
    for(int i=0; i<ccs.rows; i++){ y[i] = 0; }

    for(int j=0; j<ccs.cols; j++){
        for(int index=ccs.JA[j]; index<ccs.JA[j+1]; index++){
            double val = ccs.V[index];
            int i = ccs.IA[index];
            y[i] += val*x[j];
            if(i!=j){
                y[j] += val*x[i];
            }
        }
    }
    return y;
}

// (e) CG algo. Ref: lecture 6 P40
double** noCG(CCS ccs, double* b, int iterations){
    Eigen::VectorXd x = Eigen::VectorXd::Zero(ccs.rows);
    Eigen::VectorXd r = Eigen::Map<Eigen::VectorXd>(b, ccs.rows); // b is from prescribed from x=[1, ... 1]
    Eigen::VectorXd p = r;
    for(int k=0; k<iterations; k++){
        Eigen::VectorXd Ap = Eigen::Map<Eigen::VectorXd>(MVM(ccs, p.data()), ccs.rows);
        double rTr = (r.transpose()*r).value();
        // std::cout<<rTr<<"/"<<(p.transpose()*Ap)<<std::endl;
        double alpha = rTr/(p.transpose()*Ap).value();
        // std::cout<<x<<"+"<<alpha<<"*"<<p<<std::endl;
        x = x+alpha*p;
        // std::cout<<x<<std::endl;
        r = r-alpha*Ap;
        double beta = (r.transpose()*r).value()/rTr;
        p = r+beta*p;
        // std::cout<<x<<std::endl;
    }
    // std::cout<<x<<std::endl;
    double** result = new double*[2];
    result[0] = x.data();
    result[1] = r.data();
    return result;
}

// (f) 2-norm
double norm2calc(double* vec, int size){
	double norm2=0.0;
	for (int i=0; i<size; i++){
		norm2+=std::pow(vec[i], 2);
	}
	norm2=std::sqrt(norm2);
	return norm2;
}
// A-norm
double normAcalc(double* ek, CCS ccs){
    Eigen::VectorXd Ae = Eigen::Map<Eigen::VectorXd>(MVM(ccs, ek), ccs.rows);
    Eigen::VectorXd e = Eigen::Map<Eigen::VectorXd>(ek, ccs.rows);
    double eTAe = e.transpose()*Ae;
    return std::sqrt(eTAe);
}

int main(int argc, char* argv[]){
    std::string file = argv[1];
    size_t iterations;
    if(std::sscanf(argv[2], "%zu", &iterations) != 1)
    throw std::runtime_error("invalid value for iterations");
    
    // (b) mtx A is already lower triangle
    FULL full = mtx_reader(file);
    
    // (a)
    CCS ccs = FULL_2_L_CCS(full);
    // check together with "toy.txt"
    // for(int i=0; i<ccs.V.size(); i++){std::cout<<ccs.V[i]<<" ";}
    // std::cout<<std::endl;
    // for(int i=0; i<ccs.IA.size(); i++){std::cout<<ccs.IA[i]<<" ";}
    // std::cout<<std::endl;
    // for(int i=0; i<ccs.JA.size(); i++){std::cout<<ccs.JA[i]<<" ";}
    // std::cout<<std::endl;
    // std::cout<<ccs.nonZeros<<" "<<ccs.V.size()<<" "<<ccs.IA.size()<<" "<<ccs.JA.size()<<std::endl;
    if(ccs.nonZeros!=ccs.V.size()){ throw std::invalid_argument("CCS V has wrong size"); }
    if(ccs.nonZeros!=ccs.IA.size()){ throw std::invalid_argument("CCS IA has wrong size"); }
    if(ccs.rows+1!=ccs.JA.size()){ throw std::invalid_argument("CCS JA should be of size n+1"); }
    
    // (c) matrix multiplication: MVM
    // (d) b = A*x
    double* x = new double[ccs.rows];
    for(int i=0; i<ccs.rows; i++){ x[i]=1; }
    double* b = MVM(ccs, x);
    // check correctness with "toy.txt"
    // for(int i=0; i<ccs.rows; i++){
    //     std::cout<<b[i]<<std::endl;
    // }
    
    // (e) non-preconditioned CG algorithm
    double** result = noCG(ccs, b, iterations);
    double* xk = result[0];
    // for(int i=0; i<ccs.rows; i++){
    //     std::cout<<xk[i]<<std::endl;
    // }
    double* r0, *rk;
    r0 = b;
    rk = result[1];
    
    // (f) error of convergence
    double rk_2norm = norm2calc(rk, ccs.rows);
    double r0_2norm = norm2calc(r0, ccs.rows);
    std::cout<<"\n||rk|| / ||r0|| is: "<<rk_2norm/r0_2norm<<std::endl;
    double* ek= new double[ccs.rows];
    int irregular = 0;
    for (int i=0; i<ccs.rows; i++){
        ek[i] = xk[i]-x[i];
        if(abs(ek[i])>0.1){ irregular++; }
    }
    double Anorm = normAcalc(ek, ccs);
    std::cout<<"A-norm is: "<<Anorm<<" (with "<<irregular<<" bad estimations)"<<std::endl;
    
    // (g)(h) write file then plot
    std::ofstream resout("./res_eor.txt", std::ios::app);
    resout<<iterations<<": "<<rk_2norm/r0_2norm<<std::endl;
    resout.close();
    std::ofstream Anout("./Anorm_eor.txt", std::ios::app);
    Anout<<iterations<<": "<<Anorm<<std::endl;
    Anout.close();
}