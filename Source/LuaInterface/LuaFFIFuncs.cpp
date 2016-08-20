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

static thread_local void *userdata_to_ptr;

int LuaStoreUserDataForPtr(lua_State *L)
{
    userdata_to_ptr = lua_touserdata(L, 1);
    return 0;
}

extern "C" void *LuaLoadUserDataForPtr()
{
    return userdata_to_ptr;
}

