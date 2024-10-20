#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>


int main()
{
	std::vector<std::vector<double>> matrix;
	std::fstream input_file("Input/MatrixA.txt");
  std::string lineString, matrixElement;
	double trace = 0.;
	double max = 0.;
  int i = 0;
	while (std::getline(input_file, lineString)){
		int j = 0;
		std::istringstream line(lineString);
  	while (std::getline(line, matrixElement, ' ')){
				if (fabs(stod(matrixElement)) > max){
				max = abs(stod(matrixElement));
			}
			if (i == j){
				trace += stod(matrixElement);
			}
			j++;
		}
		i++;
	}

	std::cout << max << std::endl;
	std::cout << trace << std::endl;
  return 0;
}

