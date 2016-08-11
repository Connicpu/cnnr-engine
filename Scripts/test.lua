local ffi, serialization = require("serialization.ffi")
local toml = require("serialization.toml")

local val = toml.parse('[test]\ntest = "test"')
assert(toml.toml_to_lua(val).test.test == "test")

print("done")
