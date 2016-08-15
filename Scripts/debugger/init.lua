local mipc = require("debugger.mipc")
local ffi = require("ffi")

-- Normally you would do this in 2 separate processes, but
-- it's just a named pipe so it will work the same.
local pid = ffi.C._getpid()
local server = mipc.open_server("debug-test")
local client = mipc.open_client("debug-test", pid)

assert(server:send("test1") == 'success')
print(client:recv())

assert(client:send("test2") == 'success')
print(server:recv())
