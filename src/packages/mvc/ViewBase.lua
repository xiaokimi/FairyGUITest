
local ViewBase = class("ViewBase", cc.Node)

function ViewBase:ctor(app, scene, name)
    self:enableNodeEvents()

    self._app = app
    self._scene = scene
    self._name = name

    --
    self:onCreate()
end

function ViewBase:onCreate()

end

function ViewBase:getApp()
    return self._app
end

function ViewBase:getScene()
    return self._scene
end

function ViewBase:getName()
    return self._name
end

function ViewBase:getResourceNode()
    return self.resourceNode_
end

return ViewBase
