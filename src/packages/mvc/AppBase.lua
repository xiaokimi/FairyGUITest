
local AppBase = class("AppBase")

function AppBase:ctor(configs)
    self.configs_ = {
        sceneRoot = "app.scenes",
        viewsRoot  = "app.views",
        defaultSceneName = "MenuScene",
    }

    for k, v in pairs(configs or {}) do
        self.configs_[k] = v
    end

    --
    if type(self.configs_.sceneRoot) ~= "table" then
        self.configs_.sceneRoot = {self.configs_.sceneRoot}
    end

    if type(self.configs_.viewsRoot) ~= "table" then
        self.configs_.viewsRoot = {self.configs_.viewsRoot}
    end

    --
    if DEBUG > 1 then
        dump(self.configs_, "AppBase configs")
    end

    if CC_SHOW_FPS then
        cc.Director:getInstance():setDisplayStats(true)
    end

    -- event
    self:onCreate()
end

function AppBase:run(initSceneName)
    initSceneName = initSceneName or self.configs_.defaultSceneName
    self:enterScene(initSceneName)
end

function AppBase:enterScene(sceneName, transition, time, more)
    local scene = self:createScene(sceneName)
    display.runScene(scene, transition, time, more)
end

function AppBase:createScene(name)
    for _, root in ipairs(self.configs_.sceneRoot) do
        local packageName = string.format("%s.%s", root, name)
        local status, scene = xpcall(function()
            return require(packageName)
        end, function(msg)
            if not string.find(msg, string.format("'%s' not found: ", packageName)) then
                print("load scene error: ", msg)
            end        
        end)

        local t = type(scene)
        if status and (t == "table" or t == "userdata") then
            return scene:create(self, name)
        end
    end

    error(string.format("AppBase:createScene() - not found scene \"%s\" in search paths \"%s\"",
        name, table.concat(self.configs_.sceneRoot, ",")), 0)
end

function AppBase:createView(scene, name)
    for _, root in ipairs(self.configs_.viewRoot) do
        local packageName = string.format("%s.%s", root, name)
        local status, view = xpcall(function()
            return require(packageName)
        end, function(msg)
            if not string.find(msg, string.format("'%s' not found: ", packageName)) then
                print("load view error: ", msg)
            end        
        end)

        local t = type(view)
        if status and (t == "table" or t == "userdata") then
            return view:create(self, scene, name)
        end
    end

    error(string.format("AppBase:createView() - not found view \"%s\" in search paths \"%s\"",
        name, table.concat(self.configs_.viewsRoot, ",")), 0)
end

function AppBase:onCreate()

end

return AppBase
