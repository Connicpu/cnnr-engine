local class = require("common.class")
local Color = require("console.color")

local Console = class("Console")

-- function Console:print(format [,args...] [,formatColor])
function Console:print(format, ...)
    local args = { ... }
    local color
    if #args > 0 and type(args[#args]) == 'table' then
        local fcolor = args[#args]
        if getmetatable(fcolor) == Color then
            table.remove(args, #args)
            color = fcolor
        end
    end

    if type(format) ~= 'string' then
        self:puts(tostring(format), color)
    else
        self:puts(string.format(format, ...), color)
    end
end

-- function Console:puts(message [,formatColor])
function Console:puts(message, color)
    if type(message) ~= 'string' then
        error("Console:puts: `message` was not a string")
    elseif type(color) ~= 'table' or getmetatable(color) ~= Color then
        error("Console:puts: `color` was not a console.color")
    end

    color = color or Color.White
    table.insert(self.queue, { message, color })
end

---------------------------------------------------
-- Implementation functions

function Console:initialize()
    self.history = {}
    self.queue = {}
    self.msgObjects = {}
    self.txtCache = {}
    self.open = false
    
    self.screenHeight = 35
    self.shownLines = 10
    self.lineHeight = 1

    self.backdropHeight = (self.lineHeight + 1) * self.shownLines
    self.openY = (self.screenHeight - self.backdropHeight) / 2
    self.closedY = (self.screenHeight + self.backdropHeight) / 2

    self.openSpeed = 20
    self.closeSpeed = 50
end

-- Called at the start of the level
function Console:resetScene(data)
    self.msgObjects = {}
    self.txtCache = {}

    self:createScene(data)
    self:createBackdrop(data)
    for i, message in ipairs(self.history) do
        self:createMessage(message)
    end
end

-- Set up the scene, including creating the camera
function Console:createScene(data)
    data.scenes:register {
        name = "console",
        ordering = 1000
    }

    data.entities:create {
        name = "ConsoleCamera",
        scene = "console",
        components = {
            transform = {},
            camera = {
                height = self.screenHeight,
                width = "auto"
            }
        }
    }
end

function Console:updateBackdrop(data)
    local transform = data.components.transform[self.backdrop]
    local position = transform.position

    local targetPos, maxChange
    if self.open then
        targetPos = self.openY
        maxChange = self.openSpeed * data.delta
    else
        targetPos = self.closedY
        maxChange = self.closeSpeed * data.delta
    end

    local diff = targetPos - position.y
    if math.abs(diff) > maxChange then
        diff = maxChange * (diff / math.abs(diff))
    end

    position.y = position.y + diff
end

function Console:createBackdrop(data)
    self.backdrop = data.entities:create {
        name = "ConsoleBackdrop",
        archetype = "console/backdrop",
        scene = "console",
        backdropHeight = self.backdropHeight
    }
end

return Console
