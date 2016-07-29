#pragma once

#include <Common/Platform.h>
#include <Common/String.h>
#include <Common/optional.h>

class DynamicLibrary
{
public:
    DynamicLibrary()
        : library(nullptr)
    {
    }

    DynamicLibrary(const char *str)
        : original_name(str)
    {
#ifdef MSVC
        if (!GetModuleHandleExA(0, str, &library))
            library = LoadLibraryA(str);
#else
        library = dlopen(str, RTLD_NOW);
#endif
        if (!library)
        {
#ifdef MSVC
            _com_error err(HRESULT_FROM_WIN32(GetLastError()));
            auto extra = " ("_s + String(err.ErrorMessage()) + ")"_s;
#else
            auto extra = ""_s;
#endif
            throw std::runtime_error{ ("Failed to load library `"_s + String(str) + "`"_s + extra).c_str() };
        }
    }

    DynamicLibrary(const DynamicLibrary &other)
        : DynamicLibrary(other.original_name.c_str())
    {
    }

    DynamicLibrary(DynamicLibrary &&other)
        : original_name(std::move(other.original_name)), library(other.library)
    {
        other.Forget();
    }

    ~DynamicLibrary()
    {
        Release();
    }

    DynamicLibrary &operator=(const DynamicLibrary &other)
    {
        this->~DynamicLibrary();
        new (this) DynamicLibrary(other);
        return *this;
    }

    DynamicLibrary &operator=(DynamicLibrary &&other)
    {
        this->~DynamicLibrary();
        new (this) DynamicLibrary(std::move(other));
        return *this;
    }

    template <typename Sym, typename ...Args>
    auto Call(const char *sym, Args &&...args) const
    {
        auto func = LoadSymbol<Sym>(sym);
        if (func)
            return func(std::forward<Args>(args)...);
        throw std::runtime_error{ "Tried to call a function that didn't exist in the DynamicLibrary" };
    }

    template <typename T>
    T LoadSymbol(const char *sym) const
    {
#ifdef MSVC
        auto addr = GetProcAddress(library, sym);
#else
        auto addr = dlsym(library, sym);
#endif
        return (T)addr;
    }

    void Release()
    {
        if (library)
        {
#ifdef MSVC
            FreeLibrary(library);
#else
            dlclose(library);
#endif
            library = nullptr;
        }
    }

    void Forget()
    {
        library = nullptr;
    }

private:
    std::string original_name;
#ifdef MSVC
    HMODULE library;
#else
    void *library;
#endif
};
