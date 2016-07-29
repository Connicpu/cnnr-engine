#include <Common/LibLoading.h>
#include <Common/CxHash.h>
#include <Engine/Engine.h>
#include <iostream>

void Run(const char *backend_name)
{
    DynamicLibrary backend(backend_name);
    DynamicLibrary engine("engine");

    while (!fs::exists(fs::current_path() / "Assets"))
        fs::current_path(fs::current_path().parent_path());

    try
    {
        EngineOptions options;
        options.backend = &backend;

        engine.Call<PFRunEngine>("RunEngine", options);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
#ifdef MSVC
        MessageBoxA(nullptr, ex.what(), "FATAL ERROR", MB_ICONERROR);
#endif
        _exit(1);
    }
}

int main(int argc, char **argv)
{
    const char *backend = "dx11";
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == "--backend"_s)
        {
            if (i + 1 < argc)
            {
                std::cerr << "--backend must be followed by a string representing a backend" << std::endl;
                return 0;
            }

            backend = argv[++i];
        }
    }
    
    try
    {
        Run(backend);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
#ifdef MSVC
        MessageBoxA(nullptr, ex.what(), "FATAL ERROR", MB_ICONERROR);
#endif
        _exit(1);
    }

    return 0;
}
