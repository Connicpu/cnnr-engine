#pragma once

#include <chrono>

struct perf_printer
{
public:
    perf_printer(const char *name);
    ~perf_printer();

private:
    const char *name;
    std::chrono::high_resolution_clock::time_point start;
};
