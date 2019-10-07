local BagWindow = require("app.views.Bag.BagWindow")

local BagScene = class("BagScene", cc.load("mvc").SceneBase)

function BagScene:onCreate()
	self.super:onCreate()

	fgui.UIPackage:addPackage("Bag")

	local view = fgui.UIPackage:createObject("Bag", "Main")
	self._groot:addChild(view)

	self._bagWindow = BagWindow:create()
	self._bagWindow:retain()

    view:getChild("bagBtn"):addClickListener(function(context)
        --self._bagWindow:doShowAnimation()
        self._bagWindow:show()
    end)
end

function BagScene:onExit()
	if not tolua.isnull(self._groot) then
		self._groot:release()
		self._groot = nil
	end

	if not tolua.isnull(self._bagWindow) then
		self._bagWindow:release()
		self._bagWindow = nil
	end

	self.super:onExit()
end

function BagScene:onCleanup()

end

return BagScene