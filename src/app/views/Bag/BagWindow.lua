local BagWindow = class("BagWindow", fgui.Window)

function BagWindow:ctor()
	self._contentPane = fgui.UIPackage:createObject("Bag", "BagWin")
	self:setContentPane(self._contentPane)

	self:center()
	self:setModal(true)

	self._list = self._contentPane:getChild("list")
	self._list:addEventListener(fgui.UIEventType.ClickItem, handler(self, self.onClickItem))
	self._list:setItemRenderer(handler(self, self.renderListItem))
	self._list:setNumItems(50)
end

-- function BagWindow:doShowAnimation()
-- 	print("----------------------------------")

-- 	self:setScale(0.1, 0.1)
-- 	self:setPivot(0.5, 0.5)

-- 	self:runAction(cc.Sequence:create(
-- 		cc.ScaleTo:create(0.3, 1.0),
-- 		cc.CallFunc:create(function()
-- 			self:show()
-- 		end)))
-- end

-- function BagWindow:doHideAnimation()
-- 	print("++++++++++++++++++++++++++++++++++++")

-- 	self:runAction(cc.Sequence:create(
-- 		cc.ScaleTo:create(0.3, 0.1),
-- 		cc.CallFunc:create(function()
-- 			self:hideImmediately()
-- 		end)))
-- end

function BagWindow:renderListItem(index, object)
	object:setIcon("Icons/i"..checkint(math.random(1, 10) - 1)..".png")
	object:setText(checkint(math.random(1, 100)))
end

function BagWindow:onClickItem(context)
	local item = context:getData()
	self._contentPane:getChild("n11"):setIcon(item:getIcon());
    self._contentPane:getChild("n13"):setText(item:getText());
end

return BagWindow