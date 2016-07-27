#pragma once

class DynamicLibrary;

struct EngineOptions
{
    const DynamicLibrary *backend;
};

extern "C" void RunEngine(const EngineOptions &options);
using PFRunEngine = decltype(RunEngine)*;
