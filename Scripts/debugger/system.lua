local System = require("engine.system")

local DebugSys = System("DebugSys")

function DebugSys:initialize()
    self.dbg = require("debugger")
end

function DebugSys:process()
    self.dbg:processEvent()
end

return DebugSys 
