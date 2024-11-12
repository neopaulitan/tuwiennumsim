#include <vector>
#include <fstream>
#include <string.h>
#include <iostream>
#include <sstream>
#include <regex>
#include <cmath>
#include <iomanip>

class properties{
public:
    long year=0;
    double CPU_transistor_count=0;
    double specint_perfm_number=0;
};

// (C)
void transistors_read(std::vector<properties> &data_list);
void specint_read(std::vector<properties> &data_list);
std::vector<std::string> split (const std::string &s, char delim) ;
// (D)
void merge_by_year(std::vector<properties> &data_list);
// (E)
void save_data(std::vector<properties> data_list);
