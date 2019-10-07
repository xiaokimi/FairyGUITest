local SceneBase = class("SceneBase", cc.Scene)

function SceneBase:ctor(app, sceneName)
	self:enableNodeEvents()

	self._app = app
	self._sceneName = sceneName

	--
	self._groot = fgui.GRoot:create(self)
    self._groot:retain()

    --
	self:onCreate()
end

function SceneBase:onCreate()
	
end

function SceneBase:getApp()
	return self._app
end

function SceneBase:getSceneName()
	return self._sceneName
end

function SceneBase:getGRoot()
	return self._groot
end

function SceneBase:onEnter()

end

function SceneBase:onExit()
	if not tolua.isnull(self._groot) then
		self._groot:release()
		self._groot = nil
	end
end

return SceneBase