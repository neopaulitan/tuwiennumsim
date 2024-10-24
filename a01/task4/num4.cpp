#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

std::vector<std::vector<double>> matgen(int n){
    std::vector<std::vector<double>> output(n, std::vector<double>(n,0));

    for (int i = 0; i <n; i++){
        for (int j = 0; j<n; j++){
            output[i][j] = (i + 1.0) * j;
        }
    }
    return output;
}

double onenorm(const std::vector<std::vector<double>>& input){
   int n = input.size();
	 double max_row_sum = 0;
	 for (int j = 0; j < n; j++){ 
		double row_sum = 0;
		for(int i = 0; i < n; i++){
			row_sum += std::abs(input[i][j]);
	 	}
		if(row_sum > max_row_sum){
		 max_row_sum = row_sum;
		}
	 }
	 return max_row_sum;
}

int main(){
    int n = 1000;
    std::vector<std::vector<double>> input = matgen(n);
    auto start = high_resolution_clock::now();
    double final = onenorm(input);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
//    std::cout << "1-norm for 1000 x 1000 matrix = " << final <<endl;
    std::cout << duration.count() <<endl;
    
    return 0;
}

