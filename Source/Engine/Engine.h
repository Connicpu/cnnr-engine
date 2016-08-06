#pragma once

class DynamicLibrary;

struct EngineOptions
{
    const DynamicLibrary *backend;
    bool catch_exceptions;
};

extern "C" void RunEngine(const EngineOptions &options);
using PFRunEngine = decltype(RunEngine)*;
