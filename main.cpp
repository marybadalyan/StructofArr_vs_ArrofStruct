#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <chrono>
#include "kaizen.h"
long long random_num_generator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<long long> dis(0, 1000000);
    return dis(gen);
}

struct Particle {
    long long position;
    long long momentum;
    long long spin;

    Particle(long long pos, long long mom, long long spn) : position(pos), momentum(mom), spin(spn) {}
};


struct ArrayOfParticles { // AoS
    std::vector<Particle> particles;

    ArrayOfParticles(long long size) {
        particles.reserve(size);
        for (int i = 0; i < size; ++i) {
            particles.emplace_back(random_num_generator(), random_num_generator(), random_num_generator());
        }
    }

    long long average_position(int size) {
        long long avg_pos = 0;
        for (const auto& particle : particles) {
            avg_pos += particle.position;
        }
        return avg_pos / size;
    }

    long long average_momentum(int size) {
         long long avg_mom = 0;
        for (const auto& particle : particles) {
            avg_mom += particle.momentum;
        }
        return avg_mom / size;
    }

    long long average_spin(int size) {
         long long  avg_spin = 0;
        for (const auto& particle : particles) {
            avg_spin += particle.spin;
        }
        return avg_spin / size;
    }
};

struct ArrayOfProperties { // SoA
    std::vector<long long> position;
    std::vector<long long> momentum;
    std::vector<long long> spin;

    ArrayOfProperties(int size) {
        position.reserve(size);
        momentum.reserve(size);
        spin.reserve(size);
        for (int i = 0; i < size; ++i) {
            position.push_back(random_num_generator());
            momentum.push_back(random_num_generator());
            spin.push_back(random_num_generator());
        }
    }

    long long average_position(int size) {
         long long avg_pos = 0;
        for (long long pos : position) {
            avg_pos += pos;
        }
        return avg_pos / size;
    }

    long long average_momentum(int size) {
         long long avg_mom = 0;
        for (long long mom : momentum) {
            avg_mom += mom;
        }
        return avg_mom / size;
    }

    long long average_spin(int size) {
         long long avg_spin = 0;
        for (long long spn : spin) {
            avg_spin += spn;
        }
        return avg_spin / size;
    }
};

std::pair<int,int> process_args(int argc, char* argv[]) {
    zen::cmd_args args(argv, argc);

    auto size_options = args.get_options("-size");
    auto iter_options = args.get_options("-iterations");

    if(size_options.empty() || iter_options.empty()) {
        zen::log("Error: --size and --iterations arguments are absent using default iterations:1000 size:100000!");
        return {100000, 1000};
    }

    return {static_cast<int>(std::atoi(size_options[0].c_str())), static_cast<int>(std::atoi(iter_options[0].c_str()))};
}



int main(int argc, char* argv[]) {

    auto [size, iterations] = process_args(argc, argv);

    // Create objects
    ArrayOfParticles array_of_structs(size);
    ArrayOfProperties struct_of_arrays(size);

    // AoS timing per property
    auto start = std::chrono::high_resolution_clock::now();
    long long aos_pos = 0;
    for (size_t i = 0; i < iterations; ++i) aos_pos = array_of_structs.average_position(size);
    auto aos_time_pos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / iterations;

    start = std::chrono::high_resolution_clock::now();
    long long aos_mom = 0;
    for (size_t i = 0; i < iterations; ++i) aos_mom = array_of_structs.average_momentum(size);
    auto aos_time_mom = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / iterations;

    start = std::chrono::high_resolution_clock::now();
    long long aos_spn = 0;
    for (size_t i = 0; i < iterations; ++i) aos_spn = array_of_structs.average_spin(size);
    auto aos_time_spn = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / iterations;

    // SoA timing per property
    start = std::chrono::high_resolution_clock::now();
    long long soa_pos = 0;
    for (size_t i = 0; i < iterations; ++i) soa_pos = struct_of_arrays.average_position(size);
    auto soa_time_pos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / iterations;

    start = std::chrono::high_resolution_clock::now();
    long long soa_mom = 0;
    for (size_t i = 0; i < iterations; ++i) soa_mom = struct_of_arrays.average_momentum(size);
    auto soa_time_mom = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / iterations;

    start = std::chrono::high_resolution_clock::now();
    long long soa_spn = 0;
    for (size_t i = 0; i < iterations; ++i) soa_spn = struct_of_arrays.average_spin(size);
    auto soa_time_spn = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / iterations;

    // Calculate total time difference
    auto avg_time_dif = (aos_time_pos + aos_time_mom + aos_time_spn) - (soa_time_pos + soa_time_mom + soa_time_spn);

    // Print table with dashes between all rows
    const int w1 = 17; // Width for first column
    const int w2 = 10; // Width for data columns
    const std::string sep = "+" + std::string(w1, '-') + "+" + std::string(w2, '-') + "+" 
                            + std::string(w2, '-') + "+" + std::string(w2, '-') + "+\n";

    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "Layout" 
              << "|" << std::setw(w2) << "Position" 
              << "|" << std::setw(w2) << "Momentum" 
              << "|" << std::setw(w2) << "Spin" << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "AoS Avg " 
              << "|" << std::right << std::setw(w2) << aos_pos 
              << "|" << std::setw(w2) << aos_mom 
              << "|" << std::setw(w2) << aos_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "SoA Avg " 
              << "|" << std::right << std::setw(w2) << soa_pos 
              << "|" << std::setw(w2) << soa_mom 
              << "|" << std::setw(w2) << soa_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "AoS Time (ns)" 
              << "|" << std::right << std::setw(w2) << aos_time_pos 
              << "|" << std::setw(w2) << aos_time_mom 
              << "|" << std::setw(w2) << aos_time_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "SoA Time (ns)" 
              << "|" << std::right << std::setw(w2) << soa_time_pos 
              << "|" << std::setw(w2) << soa_time_mom 
              << "|" << std::setw(w2) << soa_time_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "Total Time Difference for " << std::setw(w1 + w2 - 3) <<  iterations  <<"|\n|" << "iterations and " << size << " elements (ns)"  
              << std::right << std::setw(w1 - 2) << avg_time_dif << std::setw(0) << "|\n"; // Span 3 columns
    std::cout << sep;

    return 0;
}
