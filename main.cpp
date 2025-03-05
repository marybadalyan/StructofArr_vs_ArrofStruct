#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <chrono>

int random_num_generator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dis(0, 1000);
    return dis(gen);
}

struct Particle {
    int position;
    int momentum;
    int spin;

    Particle(int pos, int mom, int spn) : position(pos), momentum(mom), spin(spn) {}
};

const int SIZE = 10000;
const int ITERATIONS = 1000;

struct ArrayOfParticles { // AoS
    std::vector<Particle> particles;

    ArrayOfParticles(int size) {
        particles.reserve(SIZE);
        for (int i = 0; i < SIZE; ++i) {
            particles.emplace_back(random_num_generator(), random_num_generator(), random_num_generator());
        }
    }

    int average_position() {
        volatile int avg_pos = 0;
        for (const auto& particle : particles) {
            avg_pos += particle.position;
        }
        return avg_pos / SIZE;
    }

    int average_momentum() {
        volatile int avg_mom = 0;
        for (const auto& particle : particles) {
            avg_mom += particle.momentum;
        }
        return avg_mom / SIZE;
    }

    int average_spin() {
        volatile int avg_spin = 0;
        for (const auto& particle : particles) {
            avg_spin += particle.spin;
        }
        return avg_spin / SIZE;
    }
};

struct ArrayOfProperties { // SoA
    std::vector<int> position;
    std::vector<int> momentum;
    std::vector<int> spin;

    ArrayOfProperties(int size) {
        position.reserve(SIZE);
        momentum.reserve(SIZE);
        spin.reserve(SIZE);
        for (int i = 0; i < SIZE; ++i) {
            position.push_back(random_num_generator());
            momentum.push_back(random_num_generator());
            spin.push_back(random_num_generator());
        }
    }

    int average_position() {
        volatile int avg_pos = 0;
        for (int pos : position) {
            avg_pos += pos;
        }
        return avg_pos / SIZE;
    }

    int average_momentum() {
        volatile int avg_mom = 0;
        for (int mom : momentum) {
            avg_mom += mom;
        }
        return avg_mom / SIZE;
    }

    int average_spin() {
        volatile int avg_spin = 0;
        for (int spn : spin) {
            avg_spin += spn;
        }
        return avg_spin / SIZE;
    }
};

int main() {
    ArrayOfParticles array_of_structs(SIZE);
    ArrayOfProperties struct_of_arrays(SIZE);

    // AoS timing per property
    auto start = std::chrono::high_resolution_clock::now();
    int aos_pos = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) aos_pos = array_of_structs.average_position();
    auto aos_time_pos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / ITERATIONS;

    start = std::chrono::high_resolution_clock::now();
    int aos_mom = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) aos_mom = array_of_structs.average_momentum();
    auto aos_time_mom = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / ITERATIONS;

    start = std::chrono::high_resolution_clock::now();
    int aos_spn = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) aos_spn = array_of_structs.average_spin();
    auto aos_time_spn = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / ITERATIONS;

    // SoA timing per property
    start = std::chrono::high_resolution_clock::now();
    int soa_pos = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) soa_pos = struct_of_arrays.average_position();
    auto soa_time_pos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / ITERATIONS;

    start = std::chrono::high_resolution_clock::now();
    int soa_mom = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) soa_mom = struct_of_arrays.average_momentum();
    auto soa_time_mom = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / ITERATIONS;

    start = std::chrono::high_resolution_clock::now();
    int soa_spn = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) soa_spn = struct_of_arrays.average_spin();
    auto soa_time_spn = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() / ITERATIONS;

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
    std::cout << "|" << std::left << std::setw(w1) << "AoS Avg (int)" 
              << "|" << std::right << std::setw(w2) << aos_pos 
              << "|" << std::setw(w2) << aos_mom 
              << "|" << std::setw(w2) << aos_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "SoA Avg (int)" 
              << "|" << std::right << std::setw(w2) << soa_pos 
              << "|" << std::setw(w2) << soa_mom 
              << "|" << std::setw(w2) << soa_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "AoS Time (ns/it)" 
              << "|" << std::right << std::setw(w2) << aos_time_pos 
              << "|" << std::setw(w2) << aos_time_mom 
              << "|" << std::setw(w2) << aos_time_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "SoA Time (ns/it)" 
              << "|" << std::right << std::setw(w2) << soa_time_pos 
              << "|" << std::setw(w2) << soa_time_mom 
              << "|" << std::setw(w2) << soa_time_spn << "|\n";
    std::cout << sep;
    std::cout << "|" << std::left << std::setw(w1) << "Total Time Diff" 
              << "|" << std::right << std::setw(w2 * 3 + 2) << avg_time_dif << std::setw(0) << "|\n"; // Span 3 columns
    std::cout << sep;

    return 0;
}