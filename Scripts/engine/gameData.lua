local sys = require("__internal.gameData")

local data = {}

function data:__index(name)
    if name == "delta" then
        return sys.getDelta()
    end
end

data.components = {}
function data.components:__index(name)
    local listWrapper = sys.getComponentList(name)
    self[name] = listWrapper
    return listWrapper
end

return data
