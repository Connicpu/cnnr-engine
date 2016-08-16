local class = require("common.class")

local System = function(priority, name, filter)
    local system = class(name)
    system.meta = {
        priority = priority,
        name = name,
        filter = filter,
        type = "System",
        default_enabled = false
    }

    function system:__initialize(__sys)
        self.__sys = sys
        self:initialize()
    end

    -- Stub functions
    function system:initialize()
    end
    function system:process()
        print("[WARNING] System `"..getmetatable(self).meta.name.."` does not implement self:process!")
    end

    return system
end

return System
