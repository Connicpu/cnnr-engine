#include "pch.h"
#include <Common/Filesystem.h>

extern "C" void *LuaFFIAlloc(size_t size)
{
    return malloc(size);
}

extern "C" void LuaFFIFree(void *ptr)
{
    free(ptr);
}

extern "C" size_t GetCurrentDir(char **path)
{
    auto cd = fs::current_path().generic_string();
    auto len = cd.size();
    *path = (char *)LuaFFIAlloc(len);
    memcpy(*path, &cd[0], len);
    return len;
}
