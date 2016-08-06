local sys = require("__internal.entitySystem")
local class = require("common.class")

local EntitySystem = function(priority, name, filter)
    local system = class(name)
    system.meta = {
        priority = priority,
        name = name,
        filter = filter,
        type = "EntitySystem",
        default_enabled = false
    }

    function system:__initialize(__sys)
        self.__sys = sys
        self:initialize()
    end
    
    function system:entities()
        return sys.iter(self.__sys)
    end

    -- Stub functions
    function system:initialize()
    end
    function system:process()
        print("[WARNING] System `"..getmetatable(self).meta.name.."` does not implement self:process!")
    end

    return system
end

return EntitySystem
