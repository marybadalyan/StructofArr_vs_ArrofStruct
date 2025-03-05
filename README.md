## AoS vs. SoA Performance Comparison for Quantum Particle Simulations
### Overview
This C++ program compares the performance of Array of Structures (AoS) and Structure of Arrays (SoA) data layouts when computing averages of quantum particle properties: position, momentum, and spin. It measures execution time for each property individually over a configurable number of iterations and outputs the results in a formatted table, including the total time difference between AoS and SoA.

The program  supports command-line arguments to set the number of particles (-size) and iterations (-iterations), falling back to defaults (10,000 and 1,000, respectively) if not provided.

### Features
Simulates a user-defined number of particles (size) with random integer values (0–1000) for position, momentum, and spin.
Computes the average of each property over a user-defined number of iterations (iterations).
Times each computation separately for AoS and SoA in nanoseconds per iteration.
Outputs results in a neatly formatted table with dashes between all rows.
Calculates the total time difference (AoS - SoA) to highlight performance disparity.
Uses argument parsing via "kaizen.h" for flexible configuration.

### Code Structure
```Particle```: Struct for AoS, holding position, momentum, and spin as integers.
```ArrayOfParticles```: AoS implementation using a std::vector<Particle>.
```ArrayOfProperties```: SoA implementation with separate std::vector<int> for each property.
```process_args```: Parses -size and -iterations from command-line arguments using zen::cmd_args.
```main```: Initializes particles, runs timing loops, and prints the table.
### Dependencies
C++11 or later (for std::chrono, range-based loops, etc.).
Standard libraries: ```iostream>```, ```<vector>```, ```<iomanip>```,```<random>```, ```<chrono>```.
Using [kaizen.h](https://github.com/heinsaar/kaizen) for command line argument parsing

## Build and Run 

```git clone https://github.com/marybadalyan/StructofArr_vs_ArrofStruct```

### Go into the repository:

```cd StructofArr_vs_ArrofStruct```

### Generate the build files:

```cmake -DCMAKE_BUILD_TYPE=Release -S . -B build```

### Build the project:

```cmake --build build --config Release```

Run the executable generated in the build directory:

```./build/StructofArr_vs_ArrofStruct```

Once compiled, run the program:

```./StructofArr_vs_ArrofStruct ----iterations [num] --size [num] ``` // num as in int 

### Example Output
```
|Layout           |Position  |Momentum  |Spin      |
+-----------------+----------+----------+----------+
|AoS Avg (int)    |       501|       499|       501|
+-----------------+----------+----------+----------+
|SoA Avg (int)    |       499|       500|       502|
+-----------------+----------+----------+----------+
|AoS Time (ns/it) |      6492|      7045|      5647|
+-----------------+----------+----------+----------+
|SoA Time (ns/it) |      4042|      2636|      3775|
+-----------------+----------+----------+----------+
|Total Time Difference for 1000                    |
|iterations and 10000 elements                 8731|
+-----------------+----------+----------+----------+

AoS Avg (int): Average values for AoS particles.
SoA Avg (int): Average values for SoA particles (differ due to random initialization).
AoS Time (ns/it): Time per iteration for each property in AoS layout.
SoA Time (ns/it): Time per iteration for each property in SoA layout.
Total Time Diff: Total AoS time minus total SoA time (ns), spanning all properties.
Interpretation
Performance: SoA typically outperforms AoS due to contiguous memory access, improving cache efficiency. In the sample, SoA is ~10-12% faster per property.
Total Time Diff: A positive value (e.g., 1500 ns) means AoS takes longer overall. Here, 1500 ns = (4100 + 4150 + 4120) - (3600 + 3650 + 3620).
Customization
Command-Line Args: Use -size and -iterations to adjust the simulation scale.
```
### Conclusion
SoA’s sequential access ensures that nearly all fetched data is utilized, leveraging the contiguity of memory to minimize wasted bandwidth and maximize cache efficiency. In contrast, AoS’s strided access retrieves unused data (momentum and spin) alongside the desired property (e.g., position), leading to inefficiencies that slow it down. This performance gap becomes pronounced for large size values, where cache effects dominate—AoS suffers from frequent cache thrashing as it exceeds cache capacity, while SoA maintains efficiency through contiguous access. This makes SoA particularly well-suited for quantum simulations focused on per-property operations, as demonstrated by the program’s timing results and total time difference.

