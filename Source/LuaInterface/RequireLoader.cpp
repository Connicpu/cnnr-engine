#include "RequireLoader.h"
#include <Common/String.h>
#include <Common/MMap.h>
#include <iostream>
#include <fstream>

std::optional<fs::path> lua_cache_dir;

static HashSet<String> module_whitelist =
{
    "base"_s,
    "bit"_s,
    "debug"_s,
    "ffi"_s,
    "io"_s,
    "jit"_s,
    "math"_s,
    "os"_s,
    "package"_s,
    "string"_s,
    "table"_s,
};

static std::optional<std::pair<fs::path, bool>> choose_path(const String &path)
{
    auto file_path = fs::current_path() / "Scripts"_s / path;
    auto file_exists = fs::exists(file_path);

    if (lua_cache_dir)
    {
        auto cached_path = *lua_cache_dir / path;
        auto cached_exists = fs::exists(cached_path);

        if (file_exists && cached_exists)
        {
            if (fs::last_write_time(cached_path) > fs::last_write_time(file_path))
            {
                return{ { cached_path, true } };
            }
            else
            {
                return{ { file_path, false } };
            }
        }
        else if (file_exists)
        {
            return{ { file_path, false } };
        }
        else if (cached_exists)
        {
            return{ { cached_path, true } };
        }
    }
    else if (file_exists)
    {
        return{ { file_path, false } };
    }

    return std::nullopt;
}

static bool try_load(lua_State *L, String path)
{
    bool was_cached;
    fs::path module_path;
    if (auto result = choose_path(path))
    {
        module_path = std::move(result->first);
        was_cached = result->second;
    }
    else
    {
        return false;
    }

    MMap file;
    if (!file.Open(module_path))
        throw std::runtime_error{ "Failed to open module file" };
    
    switch (luaL_loadbuffer(L, file.GetMemory<char>(), file.GetLength(), path.c_str()))
    {
        case 0:
        {
            break;
        }
        case LUA_ERRSYNTAX:
        {
            auto msg = ("Syntax Error: "_s + String::from_lua(L, -1)).into_stdstring();
            throw std::runtime_error{ msg };
        }
        case LUA_ERRMEM:
        {
            auto msg = ("Memory Allocation Error: "_s + String::from_lua(L, -1)).into_stdstring();
            throw std::runtime_error{ msg };
        }
        default:
            unreachable();
    }

    // Save the cache file
    if (!was_cached && lua_cache_dir)
    {
        if (!fs::exists(*lua_cache_dir))
            fs::create_directories(*lua_cache_dir);

        std::ofstream output(*lua_cache_dir / path, std::ios::binary);
        auto err = lua_dump(L, [](lua_State *L, const void *data, size_t sz, void *ud) -> int
        {
            auto &output = *(std::ofstream *)ud;
            if (!output.write((const char *)data, sz))
                return 1;
            return 0;
        }, &output);
        if (err)
        {
            output.close();
            std::cerr << "[WARNING] Failed to save cache file `" << path << "`" << std::endl;
        }
    }

    return 1;
}

static int require(lua_State *L)
{
    if (lua_gettop(L) < 1)
    {
        lua_pushnil(L);
        return 1;
    }

    if (lua_type(L, 1) != LUA_TSTRING)
    {
        return luaL_typerror(L, 1, "string");
    }

    // Check the loaded table
    lua_getfield(L, LUA_GLOBALSINDEX, "package");
    lua_getfield(L, -1, "loaded");
    lua_pushvalue(L, 1);
    lua_rawget(L, -2);
    if (lua_type(L, -1) != LUA_TNIL)
        return 1;
    lua_pop(L, 3);

    // Get the requested module
    auto module = String::from_lua(L, 1).into_owned();

    // Look for the module in the precompiled pack
    // TODO

    // Look for the module in the Scripts folder
    bool err = false;
    try
    {
        auto module_path = module.clone().replace('.', '/');
        if (try_load(L, module_path.clone()))
            return 1;
        if (try_load(L, module_path + ".lua"_s))
            return 1;
        if (try_load(L, module_path + "/init.lua"_s))
            return 1;
    }
    catch (const std::exception &ex)
    {
        ("An error occurred while loading module `"_s + module + "`.\n"_s + ex.what()).push_lua(L);
        module = ""_s; // We have to free module before letting lua do a longjmp
        return lua_error(L);
    }

    // Filter whitelisted modules for the fallback
    if (module_whitelist.find(module) == module_whitelist.end())
    {
        ("Failed to find module "_s + module).push_lua(L);
        module = ""_s; // We have to free module before letting lua do a longjmp
        return lua_error(L);
    }

    // Fall back to the default `require`
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_pushvalue(L, 1);
    lua_call(L, 1, 1);
    return 1;
}

void RegisterRequireLoader(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "require");
    lua_pushcclosure(L, require, 1);
    lua_setfield(L, LUA_GLOBALSINDEX, "require");
}
