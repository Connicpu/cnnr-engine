local ffi, serialization = require("serialization.ffi")
local toml = require("serialization.toml")

local temp = {
    value = ffi.new("struct JsonValue *[1]"),
    const_value = ffi.new("struct const JsonValue *[1]"),
    str = ffi.new("struct RustStr[1]"),
    -- Maybe more later if I actually build up the json api
    toml_value = ffi.new("struct TomlValue *[1]")
}

local function parse(data)
    temp.str[0].data = data
    temp.str[0].len = #data
    if serialization.json_parse_text(temp.str, temp.value) then
        return ffi.gc(temp.value[0], serialization.json_free_value)
    else
        assert(serialization.json_get_string(temp.value[0], temp.str))
        local err = ffi.string(temp.str[0].data, temp.str[0].len)
        serialization.json_free_value(temp.value[0])
        error(err)
    end
end

local function serialize(value)
    serialization.json_serialize_text(value, temp.value)
    local val = temp.value[0]
    serialization.json_get_string(val, temp.str)
    local result = ffi.string(temp.str[0].data, temp.str[0].len)
    serialization.json_free_value(val)
    return result
end

local function serialize_pretty(value)
    serialization.json_serialize_text_pretty(value, temp.value)
    local val = temp.value[0]
    serialization.json_get_string(val, temp.str)
    local result = ffi.string(temp.str[0].data, temp.str[0].len)
    serialization.json_free_value(val)
    return result
end

local function json_to_toml(value)
    serialization.json_to_toml(value, temp.toml_value)
    return ffi.gc(temp.toml_value[0], serialization.toml_free_value)
end

return {
    parse = parse,
    serialize = serialize,
    json_to_toml = json_to_toml
}
