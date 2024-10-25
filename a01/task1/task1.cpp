#include "task1.hpp"

int main(){
    std::vector<properties> data_list;

    // (C)
    std::cout<<"transistors data:"<<std::endl;
    int transistors_data_num=0;
    transistors_read(data_list);
    transistors_data_num=data_list.size();
    for(int i=0; i<data_list.size(); i++){
        std::cout<<i<<":"<<data_list[i].year<<" "<<data_list[i].CPU_transistor_count<<std::endl;
    }

    std::cout<<"specint data:"<<std::endl;
    int specint_data_num=0;
    specint_read(data_list);
    specint_data_num=data_list.size()-transistors_data_num;
    for(int i=transistors_data_num; i<data_list.size(); i++){
        std::cout<<i-transistors_data_num<<":"<<data_list[i].year<<" "<<data_list[i].specint_perfm_number<<std::endl;
    }

    // (D)
    std::cout<<"merge entries by year:"<<std::endl;
    merge_by_year(data_list);
    for(int i=0; i<data_list.size(); i++){
        std::cout<<i<<":"<<data_list[i].year<<" "<<data_list[i].CPU_transistor_count<<std::endl;
    }

    // (E)
    save_data(data_list);
}

// (C)
void transistors_read(std::vector<properties> &data_list){
    std::ifstream tr;
    tr.open("./transistors.dat");
    std::string line;
    int index=1;
    while(std::getline(tr, line)){
        if(line.find("#")!=std::string::npos){
            // C4: ignore all after #
            line=split(line, '#')[0];
        }

        // if line has information before #
        if(line.length()>0){
            properties p;
            // C1: round to lower integer
            long year=(long)stod(split(line, ' ')[0]);
            double CPU_transistor_count=0;
            for(int i=1; i<split(line, ' ').size(); i++){
                if(split(line, ' ')[i].length()!=0){
                    // C2
                    CPU_transistor_count=stod(split(line, ' ')[i])*1e3;
                    // C3
                    CPU_transistor_count=std::round(CPU_transistor_count);
                    break;
                }
            }
            p.year=year;
            p.CPU_transistor_count=CPU_transistor_count;
            data_list.push_back(p);
        }
        index++;
    }

    tr.close();
};

void specint_read(std::vector<properties> &data_list){
    std::ifstream tr;
    tr.open("./specint.dat");
    std::string line;
    int index=1;
    while(std::getline(tr, line)){
        if(line.find("#")!=std::string::npos){
            // C4: ignore all after #
            line=split(line, '#')[0];
        }

        // if line has information before #
        if(line.length()>0){
            properties p;
            // C1: round to lower integer
            long year=(long)stod(split(line, ' ')[0]);
            double specint_perfm_number=0;
            for(int i=1; i<split(line, ' ').size(); i++){
                if(split(line, ' ')[i].length()!=0){
                    // C2
                    specint_perfm_number=stod(split(line, ' ')[i])*1e-3;
                    // C3
                    specint_perfm_number=std::round(specint_perfm_number);
                    break;
                }
            }
            p.year=year;
            p.specint_perfm_number=specint_perfm_number;
            data_list.push_back(p);
        }
        index++;
    }

    tr.close();
};

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

// (D)
void merge_by_year(std::vector<properties> &data_list){
    // D1: sort out all the years
    std::vector<properties> new_list;
    std::vector<int> year_list;
    for(int i=0; i<data_list.size(); i++){
        if(std::find(year_list.begin(), year_list.end(), data_list[i].year)!=year_list.end()){
            // this year is recorded
            continue;
        }
        else{
            // this year is not yet recorded
            properties p;
            p.year=data_list[i].year;
            year_list.push_back(p.year);
            new_list.push_back(p);
        }
    }

    for(int i=0; i<new_list.size(); i++){
        // D2: average by year
        int year=new_list[i].year;
        double sum_CPU_transistor_count=0;
        double sum_specint_perfm_number=0;
        int count_CPU_transistor_count=0;
        int count_specint_perfm_number=0;
        for(int j=0; j<data_list.size(); j++){
            if(data_list[j].year==year){
                count_CPU_transistor_count +=(data_list[j].CPU_transistor_count!=0)? 1: 0;
                count_specint_perfm_number +=(data_list[j].specint_perfm_number!=0)? 1: 0;
                sum_CPU_transistor_count+=data_list[j].CPU_transistor_count;
                sum_specint_perfm_number+=data_list[j].specint_perfm_number;
            }
        }
        // std::cout<<year<<" "<<sum_CPU_transistor_count<<"/"<<count_CPU_transistor_count<<" "<<sum_specint_perfm_number<<"/"<<count_specint_perfm_number<<std::endl;
        new_list[i].CPU_transistor_count =(count_CPU_transistor_count==0)? 0: sum_CPU_transistor_count/count_CPU_transistor_count;
        new_list[i].specint_perfm_number =(count_specint_perfm_number==0)? 0: sum_specint_perfm_number/count_specint_perfm_number;
    }

    // D3: remove old entries (clean memory)
    data_list.clear();
    data_list.shrink_to_fit();
    
    // D4: store the new single-year entry
    data_list=new_list;
}

// (E)
void save_data(std::vector<properties> data_list){
    std::ofstream Fout("./COMPUTER.dat");
    for(int i=0; i<data_list.size(); i++){
        Fout<<std::setw(10)<<data_list[i].year<<","<<std::setw(15)<<data_list[i].CPU_transistor_count<<","<<std::setw(15)<<data_list[i].specint_perfm_number<< " \n";
    }
    Fout.close();
}
