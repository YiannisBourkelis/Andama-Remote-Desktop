#ifndef BENCH_H
#define BENCH_H

#include <chrono>
#include <string>

class Bench
{
public:
    explicit Bench(std::string Description);
    ~Bench();

    std::chrono::high_resolution_clock::time_point startBench;
    std::chrono::high_resolution_clock::time_point endBench;
    std::string description;
};

#endif // BENCH_H
