#pragma once

#include "IncludeLua.h"
#include <Common/Filesystem.h>
#include <Common/optional.h>

extern std::optional<fs::path> lua_cache_dir;

void RegisterRequireLoader(lua_State *L);
