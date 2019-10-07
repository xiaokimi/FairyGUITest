
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

function MainScene:onCreate()
    -- add background image
    display.newSprite("HelloWorld.png")
        :move(display.center)
        :addTo(self)

    -- add HelloWorld label
    cc.Label:createWithSystemFont("Hello World", "Arial", 40)
        :move(display.cx, display.cy + 200)
        :addTo(self)
end

function MainScene:onEnter()
    fgui.UIPackage:addPackage("MainMenu")

    --local _groot = fgui.GRoot:create(display.getRunningScene())
    --print(type(_groot))
    --_groot:retain()

    --local sprite = fgui.UIPackage:createObject("MainMenu", "CloseButton")
    --sprite:setPosition(100, 100)
    --_groot:addChild(sprite)
end

function MainScene:onExit()

end

return MainScene
