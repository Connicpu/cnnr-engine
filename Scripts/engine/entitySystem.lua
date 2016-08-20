local sys = require("__internal.entitySystem")
local class = require("common.class")

local EntitySystem = function(priority, name)
    local system = class(name)
    system.meta = {
        priority = priority,
        name = name,
        type = "EntitySystem",
        default_enabled = false
    }

    system.__index = system

    function system:__initialize(__sys)
        self.__sys = __sys
        self:initialize()
    end

    local function iterStep(__sys)
        if sys.iterStep(__sys) then
            return sys.iterValue(__sys)
        end
    end
    
    function system:entities()
        sys.iterReset(self.__sys)
        return iterStep, self.__sys
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
