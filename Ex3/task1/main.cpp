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

struct CCS{
    int rows;
    int cols;
    int nonZeros;
    std::vector<double> V;
    std::vector<int> IA;
    std::vector<int> JA;
};

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
}