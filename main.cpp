#include <iostream>
#include <vector>
#include <iomanip>
#include <random>

int random_num_generator(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<int> dis(0, 1000);
    return dis(gen);
}

struct Particle{
    int position;
    int momentum; //mass times velocity
    int spin;

    Particle(int pos, int mom, int spn) : position(pos), momentum(mom), spin(spn) {}
};

struct Properties{
    std::vector<int> position;
    std::vector<int> momentum;
    std::vector<int> spin;

    Properties(int size){
        position.reserve(size);
        momentum.reserve(size);
        spin.reserve(size);
    }   
    int average_positon(){
        int avg_pos = 0;
        for(int i = 0;i < position.size();++i){
            avg_pos += position[i];
        }
        return avg_pos;
    }
    int average_momentum(){
        int avg_mom = 0;
        for(int i = 0;i < momentum.size();++i){
            avg_mom += momentum[i];
        }
        return avg_mom;
    }
    int average_spin(){
        int avg_spin = 0;
        for(int i = 0;i < spin.size();++i){
            avg_spin += spin[i];
        }
        return avg_spin;
    }
};

int average_positon(const std::vector<Particle>& particles){
    int avg_pos = 0;
    for(int i = 0;i < particles.size();++i){
        avg_pos += particles[i].position;
    }
}
int average_momentum(const std::vector<Particle>& particles){
    int avg_pos = 0;
    for(int i = 0;i < particles.size();++i){
        avg_pos += particles[i].momentum;
    }
}
int average_spin(const std::vector<Particle>& particles){
    int avg_pos = 0;
    for(int i = 0;i < particles.size();++i){
        avg_pos += particles[i].spin;
    }
}

const int SIZE = 10000;
const int ITEARTIONS = 1000;
int main(){
    std::vector<Particle> array_of_structs;
    
    for(int i = 0; i < SIZE; ++i)
    {
        array_of_structs.push_back({random_num_generator(), random_num_generator(), random_num_generator()});
    }
    
    Properties struct_of_arrays(SIZE);
    for(int i = 0; i < SIZE; ++i)
    {
        struct_of_arrays.position.push_back(random_num_generator());
        struct_of_arrays.momentum.push_back(random_num_generator());
        struct_of_arrays.spin.push_back(random_num_generator());
    }

    for (int i = 0 ; i < ITEARTIONS;++i){
        std::cout << "Iteration: " << i << std::endl;
        std::cout << "Average position of array of structs: " << average_positon(array_of_structs) << std::endl;
        std::cout << "Average momentum of array of structs: " << average_momentum(array_of_structs) << std::endl;
        std::cout << "Average spin of array of structs: " << average_spin(array_of_structs) << std::endl;
        std::cout << "Average position of struct of arrays: " << struct_of_arrays.average_positon() << std::endl;
        std::cout << "Average momentum of struct of arrays: " << struct_of_arrays.average_momentum() << std::endl;
        std::cout << "Average spin of struct of arrays: " << struct_of_arrays.average_spin() << std::endl;
    }

    return 0;
}