local toml = require("serialization.toml")
local json = require("serialization.json")

local val = toml.parse('[test]\ntest = "test"')
assert(toml.toml_to_lua(val).test.test == "test")
print(toml.serialize(val))

val = json.parse('{"test":{"test":"test"}}')
assert(toml.toml_to_lua(json.json_to_toml(val)).test.test == "test")
print(json.serialize(val))

print("done")
