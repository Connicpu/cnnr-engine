local System = require("engine.system")

local ConsoleSys = System("ConsoleSys")

function ConsoleSys:initialize()
    self.console = require("debugger")
end

function ConsoleSys:process()
    self.dbg:processEvent()
end

return ConsoleSys 
