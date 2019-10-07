local MenuScene = class("MenuScene", cc.load("mvc").SceneBase)

function MenuScene:onCreate()
    self.super:onCreate()

    fgui.UIPackage:addPackage("MainMenu")
    local closeButton = fgui.UIPackage:createObject("MainMenu", "CloseButton")
    closeButton:setPosition(self._groot:getWidth() - closeButton:getWidth() - 10, self._groot:getHeight() - closeButton:getHeight() - 10)
    closeButton:addRelation(self._groot, fgui.RelationType.Right_Right)
    closeButton:addRelation(self._groot, fgui.RelationType.Bottom_Bottom)
    closeButton:setSortingOrder(1000)
    self._groot:addChild(closeButton)

    local view = fgui.UIPackage:createObject("MainMenu", "Main")
    self._groot:addChild(view)

    view:getChild("n10"):addClickListener(function(context)
        local app = self:getApp()
        app:enterScene("BagScene")
    end)
end

function MenuScene:onCleanup()

end

return MenuScene