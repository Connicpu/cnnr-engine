local mipc = require("debugger.mipc")

local dbg = {}

function dbg:initialize()
    self.mipc = mipc.open_server("cnnr-lua-debugger")

    while true do
        local stat, data = self.mipc:recv()
        print(stat, data)
        if stat ~= 'success' then break end
        self.mipc:send(data)
    end
end

return dbg
