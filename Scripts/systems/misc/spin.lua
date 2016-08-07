-- Dependencies
local Transform = require("engine.components.transform")
local EntitySystem = require("engine.entitySystem")
local filter = require("engine.filter")

-- Set up the system
local spinFilter = filter.components("transform", "spin")
local SpinEntities = EntitySystem("SpinEntities", spinFilter)

-- This function is where we set up data into self when the
-- system is initialized (on each level load)
function SpinEntities:initialize()
    self.speed = 0.5 -- radians per second
end

-- Define the process function
function SpinEntities:process(data)
    for entity in self:entities() do
        -- Rotate all of the transforms by delta*speed
        local transform = data.components.transform[entity]
        transform.rotation = transform.rotation + data.delta * self.speed
    end
end

-- We have to return the system from this module
return SpinEntities

