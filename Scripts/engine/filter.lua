local sys = require("__internal.filter")

local function components(...)
    local filter = nil
    for i, name in ipairs({...}) do
        if filter then
            filter = filter + sys.component(name)
        else
            filter = sys.component(name)
        end
    end
    return filter
end

return {
    component = sys.component,
    components = components
}
