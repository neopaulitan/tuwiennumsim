#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

std::vector<std::vector<double>> readmat(const std::string &filename){
    std::vector<std::vector<double>> output;
    std::ifstream file(filename);

    std::string line;

	if (!file.is_open()){
		std::cerr <<"naw"<<std::endl;
		return {};
	}

    while(std::getline(file,line)){
        std::vector<double> row;
        std::istringstream iss(line);
        double value;

        while (iss >> value){
            row.push_back(value);
        }
        output.push_back(row);
    }
    file.close();
    return output;
}

double trace(const std::vector<std::vector<double>>& input){
    int n = input.size();
    double sum = 0;
    for (int i = 0; i<n; i++){
        sum += input[i][i];
    }
    return sum;
}

double onenorm(const std::vector<std::vector<double>>& input){
	int n = input.size();
	double sum = 0;
	for (int i = 0; i<n;i++){
		double max = input[0][i];
		for (int j = 0; j < n; j++){
			if (max < input[j][i]){
				max = input[j][i];
			}
		}
		sum += max;
	}
	return sum;
}

int main(){
    std::string filename = "./task3/MatrixA.txt";
    std::vector<std::vector<double>> input = readmat(filename);
    double onenormd = onenorm(input);
    double traced = trace(input);
    std::cout << "1-norm[A] = " << onenormd << std::endl;
    std::cout << "trace[A] = " << traced << std::endl;
    
    return 0;
}

