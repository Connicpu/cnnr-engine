local class = require("common.class")
local mipc = require("debugger.mipc")
local json = require("serialization.json")

local Debugger = class("Debugger")

function Debugger:initialize()
    self.mipc = mipc.openServer("cnnr-lua-debugger")
    self.state = { mode = "scan" }
    self.breakpoints = {}
end

function Debugger:close()
    self:stopTracing()
    self.mipc:close()
end

function Debugger:startTracing(events)
    local function trace(event, line)
        self:onTrace(event, line, 2)
    end
    debug.sethook(trace, events or "crl")
end

function Debugger:stopTracing()
    debug.sethook()
end

function Debugger:setBreakpoint(bp)
    if self.breakpoints[bp.file] then
        self.breakpoints[bp.file] = {}
    end
    self.breakpoints[bp.file][bp.line] = true
end

function Debugger:removeBreakpoint(bp)
    local file = self.breakpoints[bp.file]
    if file then
        file[bp.line] = nil
        if #file == 0 then
            self.breakpoints[bp.file] = nil
        end
    end
end

function Debugger:isBreakpoint(source, line)
    local file = self.breakpoints[source]
    if not file then return false end
    if not file[line] then return false end
    return true
end

function Debugger:send(data)
    if self.mipc:send(json.serialize(json.lua_to_json(data))) ~= 'success' then
        self:close()
    end
end

function Debugger:processEvent(blocking, hookdepth)
    local status, data
    if blocking then
        status, data = self.mipc:recv()
    else
        status, data = self.mipc:tryRecv()
    end

    if status == "disconnected" then
        self:close()
        return
    elseif status == "empty" then
        return
    end

    data = json.json_to_lua(json.parse(data))

    if data.event == "getTrace" then
        if not hookdepth then
            self:send({
                event = "stacktrace",
                success = false
            })
        end

        local trace = debug.traceback(nil, hookdepth + 1)
        self:send({
            event = "stacktrace",
            success = true,
            data = {
                stacktrace = trace
            }
        })
    elseif data.event == "continue" then
        self.paused = false
    elseif data.event == "stepOver" then
        self:stepOver()
    elseif data.event == "stepInto" then
        self:stepInto()
    elseif data.event == "setBreakpoints" then
        for i, bp in ipairs(data.data) do
            self:setBreakpoint(bp)
        end
    elseif data.event == "removeBreakpoints" then
        for i, bp in ipairs(data.data) do
            self:removeBreakpoint(bp)
        end
    else
        error("Unknown event received from debug adapter")
    end
end

function Debugger:breakpoint(dbginfo, hookdepth)
    self.state.paused = true

    local data = json.lua_to_json({
        event = "break",
        data = {
            file = dbginfo.source,
            line = dbginfo.currentline
        }
    })
    self.mipc:send(json.serialize(data))

    while self.state.paused do
        self:processEvent(true, hookdepth + 1)
    end
end

function Debugger:onTrace(event, line, hookdepth)
    local info = debug.getinfo(hookdepth + 1)
    if event == "call" and info.what == "C" then
        return
    end

    if self.state.mode == "stepOver" then
        self:onStepOver(event, info, hookdepth + 1)
    elseif self.state.mode == "stepInto" then
        self:onStepInto(event, info, hookdepth + 1)
    elseif self.state.mode == "scan" then
        self:onScan(event, info, line, hookdepth + 1)
    else
        error("Debugger is in an unknown state: "..self.state.mode)
    end
end

function Debugger:stepOver()
    self.state.mode = "stepOver"
    self.state.level = 0
end

function Debugger:onStepOver(event, info, hookdepth)
    if event == "call" then
        self.state.level = self.state.level + 1
    elseif event == "return" then
        self.state.level = self.state.level - 1
    elseif event == "line" and self.state.level <= 0 then
        self.state.mode = "scan"
        self:breakpoint(info, hookdepth + 1)
    end
end

function Debugger:stepInto()
    self.state.mode = "stepInto"
end

function Debugger:onStepInto(event, info, hookdepth)
    if event == "line" then
        self.state.mode = "scan"
        self:breakpoint(info, hookdepth + 1)
    end
end

function Debugger:onScan(event, info, line, hookdepth)
    if event == "line" then
        if self:isBreakpoint(info.source, line) then
            self:breakpoint(info, hookdepth + 1)
        end
    end
end

return Debugger()
