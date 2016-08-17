local ffi = require("ffi")

ffi.cdef[[
    size_t GetCurrentDir(char **path);
    void *LuaFFIAlloc(size_t size);
    void LuaFFIFree(void *ptr);
]]
local engine = ffi.load("engine")

local function getCurrDir()
    local bp_temp = ffi.new("char *[1]")
    local len = engine.GetCurrentDir(bp_temp)
    local path = ffi.string(bp_temp[0], len)
    engine.LuaFFIFree(bp_temp[0])
    return path
end

return {
    getCurrDir = getCurrDir
}
