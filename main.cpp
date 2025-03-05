#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <chrono>

int random_num_generator(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dis(0, 1000);
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
        volatile int avg_pos = 0;
        for(int i = 0;i < position.size();++i){
            avg_pos += position[i];
        }
        return avg_pos;
    }
    int average_momentum(){
        volatile int avg_mom = 0;
        for(int i = 0;i < momentum.size();++i){
            avg_mom += momentum[i];
        }
        return avg_mom;
    }
    int average_spin(){
        volatile int avg_spin = 0;
        for(int i = 0;i < spin.size();++i){
            avg_spin += spin[i];
        }
        return avg_spin;
    }
};

int average_positon(const std::vector<Particle>& particles){
    volatile int avg_pos = 0;
    for(int i = 0;i < particles.size();++i){
        avg_pos += particles[i].position;
    }
    return avg_pos;
}
int average_momentum(const std::vector<Particle>& particles){
    volatile int avg_mom = 0;
    for(int i = 0;i < particles.size();++i){
        avg_mom += particles[i].momentum;
    }
    return avg_mom;
}
int average_spin(const std::vector<Particle>& particles){
    volatile int avg_spin = 0;
    for(int i = 0;i < particles.size();++i){
        avg_spin += particles[i].spin;
    }
    return avg_spin;
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

    auto start = std::chrono::high_resolution_clock::now();
    average_positon(array_of_structs);
    average_momentum(array_of_structs);
    average_spin(array_of_structs);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time for array of structs: " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << "ms\n";

    start = std::chrono::high_resolution_clock::now();
    struct_of_arrays.average_positon();
    struct_of_arrays.average_momentum();
    struct_of_arrays.average_spin();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time for struct of arrays: " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << "ms\n";

    return 0;
}