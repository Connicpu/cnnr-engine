local dbg = require("debugger")
dbg:startTracing()

local registerComponents = require("components")
local registerSystems = require("systems")

return function(comreg, sysreg)
    registerComponents(comreg)
    registerSystems(sysreg)
end
