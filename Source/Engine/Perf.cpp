#include "pch.h"
#include "Perf.h"

perf_printer::perf_printer(const char *name)
    : name(name), start(std::chrono::high_resolution_clock::now())
{
}

perf_printer::~perf_printer()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "[PERF] " << name << " ran in " << dur.count() / 1000000.0 << "ms" << std::endl;
}
