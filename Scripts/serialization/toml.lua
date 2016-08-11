local ffi, serialization = require("serialization.ffi")

local temp = {
    value = ffi.new("struct Value *[1]"),
    const_value = ffi.new("struct const Value *[1]"),
    str = ffi.new("struct RustStr[1]"),
    int = ffi.new("int64_t[1]"),
    double = ffi.new("double[1]"),
    bool = ffi.new("bool[1]"),
    array = ffi.new("struct Array *[1]"),
    const_array = ffi.new("struct const Array *[1]"),
    table = ffi.new("struct Table *[1]"),
    const_table = ffi.new("struct const Table *[1]"),
    table_iter = ffi.new("struct TableIterator *[1]")
}

local function toml_to_lua(value)
    if serialization.toml_get_string(value, temp.str) then
        return ffi.string(temp.str[0].data, temp.str[0].len)

    elseif serialization.toml_get_i64(value, temp.int) then
        return tonumber(temp.int[0])

    elseif serialization.toml_get_f64(value, temp.double) then
        return tonumber(temp.double[0])

    elseif serialization.toml_get_datetime(value, temp.str) then
        return ffi.string(temp.str[0].data, temp.str[0].len)

    elseif serialization.toml_get_bool(value, temp.bool) then
        return temp.bool[0]

    elseif serialization.toml_get_array(value, temp.const_array) then
        local t_ary = temp.const_array[0]
        local len = serialization.toml_array_len(t_ary)

        local ary = { n = len }
        for i = 1, len do
            assert(serialization.toml_array_get(t_ary, i - 1, temp.const_value))
            ary[i] = toml_to_lua(temp.const_value[0])
        end
        return ary

    -- Try to read out a table
    elseif serialization.toml_get_table(value, temp.const_table) then
        local t_tbl = temp.const_table[0]
        -- Make an iterator and give it a finalizer in case
        -- an inner call to toml_to_lua raises an error
        local t_iter = ffi.gc(
            serialization.toml_table_get_iterator(t_tbl),
            serialization.toml_table_iterator_free
        )

        local tbl = {}
        -- Go through the iterator results
        while serialization.toml_table_iterator_next(t_iter, temp.str, temp.const_value) do
            -- Turn the key into a lua string
            local key = ffi.string(temp.str[0].data, temp.str[0].len)
            -- Recurse to resolve that value
            local val = toml_to_lua(temp.const_value[0])
            -- Assign into our new table
            tbl[key] = val
        end

        serialization.toml_table_iterator_free(ffi.gc(t_iter, nil))
        return tbl

    end
    error("Invalid toml value (but how???)")
end

local function inner_lua_to_toml(data)
    local t = type(data)
    if t == "string" then
        temp.str[0].data = data
        temp.str[0].len = #data
        return serialization.toml_new_string(temp.str)
    end
    error("Invalid type being serialized")
end

local function lua_to_toml(data)
    local val = inner_lua_to_toml(data)
    return ffi.gc(val, serialization.toml_free_value)
end

local function parse(data)
    temp.str[0].data = data
    temp.str[0].len = #data
    if serialization.toml_parse_text(temp.str, temp.value) then
        return ffi.gc(temp.value[0], serialization.toml_free_value)
    else
        assert(serialization.toml_get_string(temp.value[0], temp.str))
        local err = ffi.string(temp.str[0].data, temp.str[0].len)
        serialization.toml_free_value(temp.value[0])
        error(err)
    end
end

local function release(value)
    serialization.toml_free_value(ffi.gc(value, nil))
end

return {
    toml_to_lua = toml_to_lua,
    lua_to_toml = lua_to_toml,
    parse = parse,
    release = release
}


