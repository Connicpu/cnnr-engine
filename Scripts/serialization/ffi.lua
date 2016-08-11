local ffi = require("ffi")

ffi.cdef[[
    struct RustStr
    {
        const char *data;
        size_t len;
    };

    struct RustStrList
    {
        struct RustStr *data;
        size_t len;
    };

    struct TomlValue;
    struct TomlArray;
    struct TomlTable;
    struct TomlTableIterator;

    /// Free a value that you have ownership of
    void toml_free_value(struct TomlValue *value);
    
    /// Create a new struct TomlValue containing a string, you have ownership of the returned value.
    /// This function will fail and return nullptr if the data is not valid UTF-8.
    struct TomlValue * toml_new_string(const struct RustStr &data);
    /// Create a new struct TomlValue containing an int64_t, you have ownership of the returned value.
    struct TomlValue * toml_new_i64(int64_t data);
    /// Create a new struct TomlValue containing an double, you have ownership of the returned value.
    struct TomlValue * toml_new_f64(double data);
    struct TomlValue * toml_new_bool(bool data);
    /// Create a new struct TomlValue containing a datetime, you have ownership of the returned value.
    /// This function will fail and return nullptr if the data is not valid UTF-8.
    struct TomlValue * toml_new_datetime(const struct RustStr &data);
    /// Create a new struct TomlValue containing an array, you have ownership of the returned value.
    struct TomlValue * toml_new_array();
    /// Create a new struct TomlValue containing a table, you have ownership of the returned value.
    struct TomlValue * toml_new_table();
    /// Make a copy of a value so you can insert it into another structure
    struct TomlValue * toml_clone(const struct TomlValue *value);
    
    /// Get a slice containing the UTF-8 data of the string.
    /// Returns false if the value is not a string.
    bool toml_get_string(const struct TomlValue *value, struct RustStr *data);
    /// Get the 64-bit integer stored in the value
    /// Returns false if the value is not an int64_t.
    bool toml_get_i64(const struct TomlValue *value, int64_t *data);
    /// Get the 64-bit float stored in the value.
    /// Returns false if the value is not a double.
    bool toml_get_f64(const struct TomlValue *value, double *data);
    /// Get a slice containing the UTF-8 data of the datetime.
    /// Returns false if the value is not a datetime.
    bool toml_get_datetime(const struct TomlValue *value, struct RustStr *data);
    bool toml_get_bool(const struct TomlValue *value, bool *data);
    
    /// Get an immutable view of the array contained in this value.
    /// Returns false if the value is not an array.
    bool toml_get_array(const struct TomlValue *value, const struct TomlArray **data);
    /// Get a mutable view of the array contained in this value.
    /// Returns false if the value is not an array.
    bool toml_get_array_mut(struct TomlValue *value, struct TomlArray **data);
    /// Get an immutable view of the table contained in this value.
    /// Returns false if the value is not a table.
    bool toml_get_table(const struct TomlValue *value, const struct TomlTable **data);
    /// Get a mutable view of the table contained in this value.
    /// Returns false if the value is not a table.
    bool toml_get_table_mut(struct TomlValue *value, struct TomlTable **data);
    
    /// Sets the value to be a String containing the data.
    /// Returns false and makes no changes to the value if the data is not valid UTF-8.
    bool toml_set_string(struct TomlValue *value, const struct RustStr &data);
    /// Sets the value to be an int64_t of the given value
    void toml_set_i64(struct TomlValue *value, int64_t data);
    /// Sets the value to be a double of the given value
    void toml_set_f64(struct TomlValue *value, double data);
    /// Sets the value to be a Datetime containing the data.
    /// Returns false and makes no changes to the value if the data is not valid UTF-8.
    bool toml_set_datetime(struct TomlValue *value, const struct RustStr &data);
    /// Sets the value to be a boolean with the given value
    void toml_set_bool(struct TomlValue *value, bool data);
    /// Sets the value to be an empty array
    void toml_set_array(struct TomlValue *value);
    /// Sets the value to be an empty table
    void toml_set_table(struct TomlValue *value);
    
    /// Removes all items from an array
    void  toml_array_clear(struct TomlArray *array);
    /// Gets the number of items in an array
    size_t toml_array_len(const struct TomlArray *array);
    /// Get the value at the given index in the array
    bool  toml_array_get(const struct TomlArray *array, size_t idx, const struct TomlValue **value);
    /// Get a mutable reference to the value at the given index in the array
    bool  toml_array_get_mut(struct TomlArray *array, size_t idx, struct TomlValue **value);
    /// Push a value onto the end of the array
    void  toml_array_push(struct TomlArray *array, struct TomlValue *value);
    /// Pop the value at the end of the array
    bool  toml_array_pop(struct TomlArray *array);
    /// Insert a value at the specified index in the array
    bool  toml_array_insert(struct TomlArray *array, size_t idx, struct TomlValue *value);
    /// Remove the value at the specified index in the array
    bool  toml_array_remove(struct TomlArray *array, size_t idx);
    
    /// Remove all items from the table
    void  toml_table_clear(struct TomlTable *table);
    /// Get the number of key-value pairs in the table
    size_t toml_table_len(const struct TomlTable *table);
    /// Get a list of all of the keys in the table. key_list should
    /// point to an array with room for all of the keys in the table.
    bool  toml_table_keys(
        const struct TomlTable *table,
        struct RustStrList *key_list
    );
    /// Get the value with the specified key from the table
    bool toml_table_get(
        const struct TomlTable *table,
        const struct RustStr &key,
        const struct TomlValue **value
    );
    /// Get a mutable value with the specified key from the table
    bool toml_table_get_mut(
        struct TomlTable *table,
        const struct RustStr &key,
        struct TomlValue **value
    );
    /// Insert a value into the table with the specified key
    bool toml_table_insert(
        struct TomlTable *table,
        const struct RustStr &key,
        struct TomlValue *value
    );
    /// Remove the value in the table with the specified key
    bool toml_table_remove(
        struct TomlTable *table,
        const struct RustStr &key
    );
    /// Get an iterator over a table
    struct TomlTableIterator * toml_table_get_iterator(
        const struct TomlTable *table
    );
    bool toml_table_iterator_next(
        struct TomlTableIterator *iter,
        struct RustStr *key,
        const struct TomlValue **value
    );
    void toml_table_iterator_free(
        struct TomlTableIterator *iter
    );
    
    bool toml_parse_text(
        const struct RustStr &data,
        struct TomlValue **output
    );
    
    void toml_serialize_text(
        const struct TomlValue *data,
        struct TomlValue **output
    );

    struct JsonValue;

    /// Free a value that you have ownership of
    void json_free_value(struct JsonValue *value);
    /// Make a copy of a value so you can insert it into another structure
    struct JsonValue * json_clone(const struct JsonValue *value);
    
    /// Get a slice containing the UTF-8 data of the string.
    /// Returns false if the value is not a string.
    bool json_get_string(const struct JsonValue *value, struct RustStr *data);

    bool json_parse_text(
        const struct RustStr &data,
        struct JsonValue **output
    );
    
    void json_serialize_text(
        const struct JsonValue *data,
        struct JsonValue **output
    );
    
    void json_serialize_text_pretty(
        const struct JsonValue *data,
        struct JsonValue **output
    );

    void toml_to_json(const struct TomlValue *input, struct JsonValue **output);
    void json_to_toml(const struct JsonValue *input, struct TomlValue **output);
]]

local lib = ffi.load("serialization")
return ffi, lib
