#pragma once

class DynamicLibrary;

struct EngineOptions
{
    const DynamicLibrary *backend;
};

extern "C" const char *RunEngine(const EngineOptions &options);
using PFRunEngine = decltype(RunEngine)*;
