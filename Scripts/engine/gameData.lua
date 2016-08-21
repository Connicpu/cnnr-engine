local sys = require("__internal.gameData")
local class = require("common.class")

local GameData = class("GameData")
local ComponentsAccessor = class("ComponentsAccessor")
local ComponentList = class("ComponentList")
local GraphicsWrapper = require("engine.graphics")

function GameData:initialize()
    self.components = ComponentsAccessor()
    self.graphics = GraphicsWrapper(sys.getGraphics())
end

function GameData:__index(name)
    if name == "delta" then
        return sys.getDelta()
    end

    return rawget(getmetatable(self), name)
end

function ComponentsAccessor:__index(name)
    local ptr = sys.getComponentList(name)
    if not ptr then return nil end

    local listWrapper = ComponentList(name, ptr)
    self[name] = listWrapper
    return listWrapper
end

function ComponentList:initialize(name, __sys)
    self.name = name
    self.__sys = __sys
end

function ComponentList:__index(entity)
    if type(entity) == 'string' then
        return getmetatable(self)[entity]
    end

    return sys.getComponent(rawget(self, '__sys'), entity)
end

function ComponentList:__tostring()
    return "ComponentList("..self.name..")"
end

return GameData()
