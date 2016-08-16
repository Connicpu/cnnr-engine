local ffi = require("ffi")

ffi.cdef[[
    struct IpcClient;

    enum IpcResult {
        MIPC_SUCCESS = 0,
        MIPC_EMPTY = 1,
        MIPC_DISCONNECTED = 2
    };

    struct IpcClient *mipc_open_server(const char *name);
    struct IpcClient *mipc_open_client(const char *name, uint32_t pid);
    void mipc_close(struct IpcClient *client);

    int mipc_send(struct IpcClient *client, const uint8_t *data, uint32_t len);
    int mipc_recv(struct IpcClient *client, uint8_t **data, size_t *len);
    int mipc_try_recv(struct IpcClient *client, uint8_t **data, size_t *len);
    void mipc_recv_free(uint8_t *data, size_t len);

    unsigned _getpid();

    struct IpcClientWrapper
    {
        struct IpcClient *ptr;
    };
]]
local lib = ffi.load("messageipc")

local temp = {
    data = ffi.new("uint8_t *[1]"),
    len = ffi.new("size_t[1]")
}

local STAT = {
    [0] = "success",
    [1] = "empty",
    [2] = "disconnected"
}

local ClientMT = {}
ClientMT.__index = ClientMT
function ClientMT:__gc()
    self:close()
end

function ClientMT:close()
    if self.ptr ~= nil then
        lib.mipc_close(self.ptr)
        self.ptr = nil
    end
end

function ClientMT:isOpen()
    return self.ptr ~= nil
end

function ClientMT:ensureOpen()
    if not self:isOpen() then
        error("mipc client has already been closed")
    end
end

function ClientMT:send(data)
    self:ensureOpen()
    return STAT[lib.mipc_send(self.ptr, data, #data)]
end

function ClientMT:recv()
    self:ensureOpen()
    local status = lib.mipc_recv(self.ptr, temp.data, temp.len)
    if status == lib.MIPC_SUCCESS then
        local data = ffi.string(temp.data[0], temp.len[0])
        return STAT[status], data
    else
        return STAT[status], nil
    end
end

function ClientMT:try_recv()
    self:ensureOpen()
    local status = lib.mipc_try_recv(self.ptr, temp.data, temp.len)
    if status == lib.MIPC_SUCCESS then
        local data = ffi.string(temp.data[0], temp.len[0])
        return STAT[status], data
    else
        return STAT[status], nil
    end
end

local ClientCT = ffi.metatype("struct IpcClientWrapper", ClientMT)

local function open_server(name)
    local ptr = lib.mipc_open_server(name)
    if ptr then
        return ClientCT(ptr)
    else
        error("Failed to open an mipc server for `"..name.."`")
    end
end

local function open_client(name, pid)
    local ptr = lib.mipc_open_client(name, pid)
    if ptr then
        return ClientCT(ptr)
    else
        error("Failed to open an mipc server for `"..name.."`@"..tostring(pid))
    end
end

return {
    open_server = open_server,
    open_client = open_client
}
