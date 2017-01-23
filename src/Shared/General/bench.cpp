#include "bench.h"
#include "iostream"


Bench::Bench(std::string Description)
{
    description = Description;
    startBench = std::chrono::high_resolution_clock::now();
}

Bench::~Bench()
{
    endBench = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( endBench - startBench ).count();
    std::cout << description << " :" << duration << "ms" << std::endl;
}
