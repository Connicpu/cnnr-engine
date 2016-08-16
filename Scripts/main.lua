local dbg = require("debugger")
dbg:initialize()

local registers = require("engine.registers")
local registerComponents = require("components")
local registerSystems = require("systems")

return function()
    local comreg = registers.ComponentRegister()
    local sysreg = registers.SystemRegister()

    registerComponents(comreg)
    registerSystems(sysreg)
end
