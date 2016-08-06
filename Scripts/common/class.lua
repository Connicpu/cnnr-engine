return function(name, base)
    name = name or "UNNAMED_LUA_CLASS"
    base = base or {}
    base.__index = base
    local class = setmetatable({
        __init = = function()
    })
end
