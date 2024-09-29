-- ++++++++ WAX BUNDLED DATA BELOW ++++++++ --

-- Will be used later for getting flattened globals
local ImportGlobals

-- Holds direct closure data (defining this before the DOM tree for line debugging etc)
local ClosureBindings = {
    function()local wax,script,require=ImportGlobals(1)local ImportGlobals return (function(...)local references = require(script.utilities.references)
local data = require(script.Bundles.data)
local services = require(script.Bundles.services)
local ContextActionService = services.ContextActionService

local connections = require(script.utilities.connections)
local customFunctions = require(script.utilities.customFunctions)

local mainLayout = require(script.layouts.main)
local get = require(script.utilities.get)

local function addComponent(name, parent, properties)
	properties.Parent = parent
	return require(script.components[name])(properties)
end

local gtaMenu = {}
function gtaMenu:Window(windowProperty)
	if customFunctions.getgenv then
		if customFunctions.getgenv().gtaMenuInstance then
			connections:deleteConnections()
			references:clear()
			customFunctions.getgenv().gtaMenuInstance = nil
		end
	else
		warn("lacking getgenv using _G (1)")
		if _G.gtaMenuInstance then
			connections:deleteConnections()
			references:clear()
			_G.gtaMenuInstance = nil
		end
	end

	assert(windowProperty.Title, ":Window missing property Title")
	assert(typeof(windowProperty.Title) == "string", ("Title accepts type string got %s"):format(typeof(windowProperty.Name)))
	assert(windowProperty.FooterLeft, ":Window missing property FooterLeft")
	assert(typeof(windowProperty.FooterLeft) == "string", ("FooterLeft accepts type string got %s"):format(typeof(windowProperty.FooterLeft)))
	assert(windowProperty.FooterRight, ":Window missing property FooterRight")
	assert(typeof(windowProperty.FooterRight) == "string", ("FooterRight accepts type string got %s"):format(typeof(windowProperty.FooterRight)))
	assert(windowProperty.Image, ":Window missing property Image")
	assert(typeof(windowProperty.Image) == "string", ("Image accepts type string got %s"):format(typeof(windowProperty.Image)))
	assert(windowProperty.ImageBackgroundColor, ":Window missing property ImageBackgroundColor")
	assert(typeof(windowProperty.ImageBackgroundColor) == "Color3", ("ImageBackgroundColor accepts type Color3 got %s"):format(typeof(windowProperty.ImageBackgroundColor)))
	assert(windowProperty.HideBind, ":Window missing property HideBind")
	assert(typeof(windowProperty.HideBind) == "EnumItem", ("HideBind accepts type EnumItem got %s"):format(typeof(windowProperty.HideBind)))
	assert(windowProperty.BackBind, ":Window missing property BackBind")
	assert(typeof(windowProperty.BackBind) == "EnumItem", ("BackBind accepts type EnumItem got %s"):format(typeof(windowProperty.BackBind)))
	assert(windowProperty.UpBind, ":Window missing property UpBind")
	assert(typeof(windowProperty.UpBind) == "EnumItem", ("UpBind accepts type EnumItem got %s"):format(typeof(windowProperty.UpBind)))
	assert(windowProperty.DownBind, ":Window missing property DownBind")
	assert(typeof(windowProperty.DownBind) == "EnumItem", ("DownBind accepts type EnumItem got %s"):format(typeof(windowProperty.DownBind)))
	assert(windowProperty.EnterBind, ":Window missing property EnterBind")
	assert(typeof(windowProperty.EnterBind) == "EnumItem", ("EnterBind accepts type EnumItem got %s"):format(typeof(windowProperty.EnterBind)))
	assert(windowProperty.LeftBind, ":Window missing property LeftBind")
	assert(typeof(windowProperty.LeftBind) == "EnumItem", ("LeftBind accepts type EnumItem got %s"):format(typeof(windowProperty.LeftBind)))
	assert(windowProperty.RightBind, ":Window missing property RightBind")
	assert(typeof(windowProperty.LeftBind) == "EnumItem", ("RightBind accepts type EnumItem got %s"):format(typeof(windowProperty.RightBind)))

	windowProperty.SizeXInOffset = windowProperty.SizeXInOffset or 300
	windowProperty.Parent = windowProperty.Parent or game.CoreGui

	local selectedTab = data.add({data = {}}, "selectedTab")
	data.add({binds = windowProperty.BlacklistedBinds or {}}, "blacklistedBinds")
	local isHidden = data.add({boolean = false}, "isHidden")
	data.add({data = {}}, "hoveredComponent")
	local currentIndex = data.add({index = 1}, "currentIndex")
	local totalIndex = data.add({total = 0})
	local isInTab = data.add({boolean = false}, "isInTab")
	local isInDropdown = data.add({boolean = false}, "isInDropdown")
	local absoluteOffset = data.add({amount = 58}, "absoluteOffset")
	if windowProperty.noOffset then
		absoluteOffset:update("amount", 0)
	end
	data.add({binds = {}}, "currentBinds")
	local windowData = data.add({data = windowProperty})
	data.add({key = {
		hide = windowProperty.HideBind,
		back = windowProperty.BackBind,
		up = windowProperty.UpBind,
		down = windowProperty.DownBind,
		enter = windowProperty.EnterBind,
		left = windowProperty.LeftBind,
		right = windowProperty.RightBind
	}}, "keys")

	ContextActionService:BindActionAtPriority("tabUp", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or get(isInTab.boolean) or get(isInDropdown.boolean) then
			return Enum.ContextActionResult.Pass
		end
		local current = get(currentIndex.index)
		if get(totalIndex.total) > 0 then
			currentIndex:update("index", math.clamp(current - 1, 1, get(totalIndex.total)))
		end
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, windowProperty.UpBind)

	ContextActionService:BindActionAtPriority("tabDown", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or get(isInTab.boolean) or get(isInDropdown.boolean) then
			return Enum.ContextActionResult.Pass
		end
		local current = get(currentIndex.index)
		if get(totalIndex.total) > 0 then
			currentIndex:update("index", math.clamp(current + 1, 1, get(totalIndex.total)))
		end
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, windowProperty.DownBind)

	ContextActionService:BindActionAtPriority("hideGTAMenu", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		isHidden:update("boolean", not get(isHidden.boolean))
		return Enum.ContextActionResult.Sink
	end,false, 1, windowProperty.HideBind)

	windowProperty.uid = windowData.uid
	if windowProperty.noOffset then
		absoluteOffset:update("amount", 0)
	end

	mainLayout(windowProperty)
	local tabButtonList = references.get("tabButtonList")
	local tabWindowList = references.get("tabWindowList")

	gtaMenu[windowProperty.uid] = {}
	local tabs = gtaMenu[windowProperty.uid]
	function tabs:Tab(tabProperty)
		tabProperty.index = get(totalIndex.total) + 1
		totalIndex:update("total", tabProperty.index)

		local tabData = data.add({data = tabProperty, index = 1})
		local totalTabIndex = data.add({total = 0})
		tabProperty.uid = tabData.uid
		tabProperty.SizeXInOffset = windowProperty.SizeXInOffset

		ContextActionService:BindActionAtPriority(tabProperty.uid.."componentUp", function(_, inputState)
			if inputState ~= Enum.UserInputState.Begin then
				return Enum.ContextActionResult.Pass
			end
			if get(isHidden.boolean) or get(isInDropdown.boolean) or not get(isInTab.boolean) or get(selectedTab.data).uid ~= tabProperty.uid then
				return Enum.ContextActionResult.Pass
			end
			local current = get(tabData.index)
			if get(totalTabIndex.total) > 0 then
				tabData:update("index", math.clamp(current - 1, 1, get(totalTabIndex.total)))
			end
			return Enum.ContextActionResult.Sink
		end,false, Enum.ContextActionPriority.High.Value, windowProperty.UpBind)

		ContextActionService:BindActionAtPriority(tabProperty.uid.."componentDown", function(_, inputState)
			if inputState ~= Enum.UserInputState.Begin then
				return Enum.ContextActionResult.Pass
			end
			if get(isHidden.boolean) or get(isInDropdown.boolean) or not get(isInTab.boolean) or get(selectedTab.data).uid ~= tabProperty.uid then
				return Enum.ContextActionResult.Pass
			end
			local current = get(tabData.index)
			if get(totalTabIndex.total) > 0 then
				tabData:update("index", math.clamp(current + 1, 1, get(totalTabIndex.total)))
			end
			return Enum.ContextActionResult.Sink
		end,false, Enum.ContextActionPriority.High.Value, windowProperty.DownBind)

		local tabWindow = addComponent("tabWindow", tabWindowList, tabProperty)
		addComponent("tabButton", tabButtonList, tabProperty)

		tabs[tabProperty.uid] = {}
		local components = tabs[tabProperty.uid]
		function components:Button(buttonProperty)
			buttonProperty.uid = tabProperty.uid
			if buttonProperty.IsEnabled == nil or buttonProperty.IsEnabled then
				buttonProperty.index = get(totalTabIndex.total) + 1
				totalTabIndex:update("total", buttonProperty.index)
			end
			return addComponent("button", tabWindow, buttonProperty)
		end
		function components:Toggle(toggleProperty)
			toggleProperty.uid = tabProperty.uid
			if toggleProperty.IsEnabled == nil or toggleProperty.IsEnabled then
				toggleProperty.index = get(totalTabIndex.total) + 1
				totalTabIndex:update("total", toggleProperty.index)
			end
			return addComponent("toggle", tabWindow, toggleProperty)
		end
		function components:Dropdown(dropdownProperty)
			dropdownProperty.uid = tabProperty.uid
			if dropdownProperty.IsEnabled == nil or dropdownProperty.IsEnabled then
				dropdownProperty.index = get(totalTabIndex.total) + 1
				totalTabIndex:update("total", dropdownProperty.index)
			end
			return addComponent("dropdown", tabWindow, dropdownProperty)
		end
		function components:Label(string)
			return addComponent("label", tabWindow, {Text = string})
		end
		function components:TextBox(textboxProperty)
			textboxProperty.uid = tabProperty.uid
			if textboxProperty.IsEnabled == nil or textboxProperty.IsEnabled then
				textboxProperty.index = get(totalTabIndex.total) + 1
				totalTabIndex:update("total", textboxProperty.index)
			end
			return addComponent("textbox", tabWindow, textboxProperty)
		end
		function components:Slider(sliderProperty)
			sliderProperty.uid = tabProperty.uid
			if sliderProperty.IsEnabled == nil or sliderProperty.IsEnabled then
				sliderProperty.index = get(totalTabIndex.total) + 1
				totalTabIndex:update("total", sliderProperty.index)
			end
			return addComponent("slider", tabWindow, sliderProperty)
		end
		function components:Keybind(keybindProperty)
			keybindProperty.uid = tabProperty.uid
			if keybindProperty.IsEnabled == nil or keybindProperty.IsEnabled then
				keybindProperty.index = get(totalTabIndex.total) + 1
				totalTabIndex:update("total", keybindProperty.index)
			end
			return addComponent("keybind", tabWindow, keybindProperty)
		end
		return tabs[tabProperty.uid]
	end
	function gtaMenu:Notify(notifyProperty)
		return addComponent("notification", references.get("notificationList"), notifyProperty)
	end
	function gtaMenu:Destroy()
		connections:deleteConnections()
		references:clear()
	end

	if customFunctions.getgenv then
		customFunctions.getgenv().gtaMenuInstance = gtaMenu
	else
		warn("lacked getgenv using _G (2)")
		_G.gtaMenuInstance = gtaMenu
	end
	return gtaMenu[windowProperty.uid]
end
return gtaMenu
end)() end,
    [3] = function()local wax,script,require=ImportGlobals(3)local ImportGlobals return (function(...)--!strict

--[[
	The entry point for the Fusion library.
]]

local PubTypes = require(script.PubTypes)
local restrictRead = require(script.Utility.restrictRead)

export type StateObject<T> = PubTypes.StateObject<T>
export type CanBeState<T> = PubTypes.CanBeState<T>
export type Symbol = PubTypes.Symbol
export type Value<T> = PubTypes.Value<T>
export type Computed<T> = PubTypes.Computed<T>
export type ForPairs<KO, VO> = PubTypes.ForPairs<KO, VO>
export type ForKeys<KI, KO> = PubTypes.ForKeys<KI, KO>
export type ForValues<VI, VO> = PubTypes.ForKeys<VI, VO>
export type Observer = PubTypes.Observer
export type Tween<T> = PubTypes.Tween<T>
export type Spring<T> = PubTypes.Spring<T>

type Fusion = {
	version: PubTypes.Version,

	New: (className: string) -> ((propertyTable: PubTypes.PropertyTable) -> Instance),
	Hydrate: (target: Instance) -> ((propertyTable: PubTypes.PropertyTable) -> Instance),
	Ref: PubTypes.SpecialKey,
	Cleanup: PubTypes.SpecialKey,
	Children: PubTypes.SpecialKey,
	Out: PubTypes.SpecialKey,
	OnEvent: (eventName: string) -> PubTypes.SpecialKey,
	OnChange: (propertyName: string) -> PubTypes.SpecialKey,

	Value: <T>(initialValue: T) -> Value<T>,
	Computed: <T>(callback: () -> T, destructor: (T) -> ()?) -> Computed<T>,
	ForPairs: <KI, VI, KO, VO, M>(inputTable: CanBeState<{[KI]: VI}>, processor: (KI, VI) -> (KO, VO, M?), destructor: (KO, VO, M?) -> ()?) -> ForPairs<KO, VO>,
	ForKeys: <KI, KO, M>(inputTable: CanBeState<{[KI]: any}>, processor: (KI) -> (KO, M?), destructor: (KO, M?) -> ()?) -> ForKeys<KO, any>,
	ForValues: <VI, VO, M>(inputTable: CanBeState<{[any]: VI}>, processor: (VI) -> (VO, M?), destructor: (VO, M?) -> ()?) -> ForValues<any, VO>,
	Observer: (watchedState: StateObject<any>) -> Observer,

	Tween: <T>(goalState: StateObject<T>, tweenInfo: TweenInfo?) -> Tween<T>,
	Spring: <T>(goalState: StateObject<T>, speed: number?, damping: number?) -> Spring<T>,

	cleanup: (...any) -> (),
	doNothing: (...any) -> ()
}

return restrictRead("Fusion", {
	version = {major = 0, minor = 2, isRelease = true},

	New = require(script.Instances.New),
	Hydrate = require(script.Instances.Hydrate),
	Ref = require(script.Instances.Ref),
	Out = require(script.Instances.Out),
	Cleanup = require(script.Instances.Cleanup),
	Children = require(script.Instances.Children),
	OnEvent = require(script.Instances.OnEvent),
	OnChange = require(script.Instances.OnChange),

	Value = require(script.State.Value),
	Computed = require(script.State.Computed),
	ForPairs = require(script.State.ForPairs),
	ForKeys = require(script.State.ForKeys),
	ForValues = require(script.State.ForValues),
	Observer = require(script.State.Observer),

	Tween = require(script.Animation.Tween),
	Spring = require(script.Animation.Spring),

	cleanup = require(script.Utility.cleanup),
	doNothing = require(script.Utility.doNothing)
}) :: Fusion

end)() end,
    [5] = function()local wax,script,require=ImportGlobals(5)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs a new computed state object, which follows the value of another
	state object using a spring simulation.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Types = require(Package.Types)
local logError = require(Package.Logging.logError)
local logErrorNonFatal = require(Package.Logging.logErrorNonFatal)
local unpackType = require(Package.Animation.unpackType)
local SpringScheduler = require(Package.Animation.SpringScheduler)
local useDependency = require(Package.Dependencies.useDependency)
local initDependency = require(Package.Dependencies.initDependency)
local updateAll = require(Package.Dependencies.updateAll)
local xtypeof = require(Package.Utility.xtypeof)
local unwrap = require(Package.State.unwrap)

local class = {}

local CLASS_METATABLE = {__index = class}
local WEAK_KEYS_METATABLE = {__mode = "k"}

--[[
	Returns the current value of this Spring object.
	The object will be registered as a dependency unless `asDependency` is false.
]]
function class:get(asDependency: boolean?): any
	if asDependency ~= false then
		useDependency(self)
	end
	return self._currentValue
end

--[[
	Sets the position of the internal springs, meaning the value of this
	Spring will jump to the given value. This doesn't affect velocity.

	If the type doesn't match the current type of the spring, an error will be
	thrown.
]]
function class:setPosition(newValue: PubTypes.Animatable)
	local newType = typeof(newValue)
	if newType ~= self._currentType then
		logError("springTypeMismatch", nil, newType, self._currentType)
	end

	self._springPositions = unpackType(newValue, newType)
	self._currentValue = newValue
	SpringScheduler.add(self)
	updateAll(self)
end

--[[
	Sets the velocity of the internal springs, overwriting the existing velocity
	of this Spring. This doesn't affect position.

	If the type doesn't match the current type of the spring, an error will be
	thrown.
]]
function class:setVelocity(newValue: PubTypes.Animatable)
	local newType = typeof(newValue)
	if newType ~= self._currentType then
		logError("springTypeMismatch", nil, newType, self._currentType)
	end

	self._springVelocities = unpackType(newValue, newType)
	SpringScheduler.add(self)
end

--[[
	Adds to the velocity of the internal springs, on top of the existing
	velocity of this Spring. This doesn't affect position.

	If the type doesn't match the current type of the spring, an error will be
	thrown.
]]
function class:addVelocity(deltaValue: PubTypes.Animatable)
	local deltaType = typeof(deltaValue)
	if deltaType ~= self._currentType then
		logError("springTypeMismatch", nil, deltaType, self._currentType)
	end

	local springDeltas = unpackType(deltaValue, deltaType)
	for index, delta in ipairs(springDeltas) do
		self._springVelocities[index] += delta
	end
	SpringScheduler.add(self)
end

--[[
	Called when the goal state changes value, or when the speed or damping has
	changed.
]]
function class:update(): boolean
	local goalValue = self._goalState:get(false)

	-- figure out if this was a goal change or a speed/damping change
	if goalValue == self._goalValue then
		-- speed/damping change
		local damping = unwrap(self._damping)
		if typeof(damping) ~= "number" then
			logErrorNonFatal("mistypedSpringDamping", nil, typeof(damping))
		elseif damping < 0 then
			logErrorNonFatal("invalidSpringDamping", nil, damping)
		else
			self._currentDamping = damping
		end

		local speed = unwrap(self._speed)
		if typeof(speed) ~= "number" then
			logErrorNonFatal("mistypedSpringSpeed", nil, typeof(speed))
		elseif speed < 0 then
			logErrorNonFatal("invalidSpringSpeed", nil, speed)
		else
			self._currentSpeed = speed
		end

		return false
	else
		-- goal change - reconfigure spring to target new goal
		self._goalValue = goalValue

		local oldType = self._currentType
		local newType = typeof(goalValue)
		self._currentType = newType

		local springGoals = unpackType(goalValue, newType)
		local numSprings = #springGoals
		self._springGoals = springGoals

		if newType ~= oldType then
			-- if the type changed, snap to the new value and rebuild the
			-- position and velocity tables
			self._currentValue = self._goalValue

			local springPositions = table.create(numSprings, 0)
			local springVelocities = table.create(numSprings, 0)
			for index, springGoal in ipairs(springGoals) do
				springPositions[index] = springGoal
			end
			self._springPositions = springPositions
			self._springVelocities = springVelocities

			-- the spring may have been animating before, so stop that
			SpringScheduler.remove(self)
			return true

			-- otherwise, the type hasn't changed, just the goal...
		elseif numSprings == 0 then
			-- if the type isn't animatable, snap to the new value
			self._currentValue = self._goalValue
			return true

		else
			-- if it's animatable, let it animate to the goal
			SpringScheduler.add(self)
			return false
		end
	end
end

local function Spring<T>(
	goalState: PubTypes.Value<T>,
	speed: PubTypes.CanBeState<number>?,
	damping: PubTypes.CanBeState<number>?
): Types.Spring<T>
	-- apply defaults for speed and damping
	if speed == nil then
		speed = 10
	end
	if damping == nil then
		damping = 1
	end

	local dependencySet = {[goalState] = true}
	if xtypeof(speed) == "State" then
		dependencySet[speed] = true
	end
	if xtypeof(damping) == "State" then
		dependencySet[damping] = true
	end

	local self = setmetatable({
		type = "State",
		kind = "Spring",
		dependencySet = dependencySet,
		-- if we held strong references to the dependents, then they wouldn't be
		-- able to get garbage collected when they fall out of scope
		dependentSet = setmetatable({}, WEAK_KEYS_METATABLE),
		_speed = speed,
		_damping = damping,

		_goalState = goalState,
		_goalValue = nil,

		_currentType = nil,
		_currentValue = nil,
		_currentSpeed = unwrap(speed),
		_currentDamping = unwrap(damping),

		_springPositions = nil,
		_springGoals = nil,
		_springVelocities = nil
	}, CLASS_METATABLE)

	initDependency(self)
	-- add this object to the goal state's dependent set
	goalState.dependentSet[self] = true
	self:update()

	return self
end

return Spring
end)() end,
    [6] = function()local wax,script,require=ImportGlobals(6)local ImportGlobals return (function(...)--!strict

--[[
	Manages batch updating of spring objects.
]]

local RunService = game:GetService("RunService")

local Package = script.Parent.Parent
local Types = require(Package.Types)
local packType = require(Package.Animation.packType)
local springCoefficients = require(Package.Animation.springCoefficients)
local updateAll = require(Package.Dependencies.updateAll)

type Set<T> = {[T]: any}
type Spring = Types.Spring<any>

local SpringScheduler = {}

local EPSILON = 0.0001
local activeSprings: Set<Spring> = {}
local lastUpdateTime = os.clock()

function SpringScheduler.add(spring: Spring)
	-- we don't necessarily want to use the most accurate time - here we snap to
	-- the last update time so that springs started within the same frame have
	-- identical time steps
	spring._lastSchedule = lastUpdateTime
	spring._startDisplacements = {}
	spring._startVelocities = {}
	for index, goal in ipairs(spring._springGoals) do
		spring._startDisplacements[index] = spring._springPositions[index] - goal
		spring._startVelocities[index] = spring._springVelocities[index]
	end

	activeSprings[spring] = true
end

function SpringScheduler.remove(spring: Spring)
	activeSprings[spring] = nil
end


local function updateAllSprings()
	local springsToSleep: Set<Spring> = {}
	lastUpdateTime = os.clock()

	for spring in pairs(activeSprings) do
		local posPos, posVel, velPos, velVel = springCoefficients(lastUpdateTime - spring._lastSchedule, spring._currentDamping, spring._currentSpeed)

		local positions = spring._springPositions
		local velocities = spring._springVelocities
		local startDisplacements = spring._startDisplacements
		local startVelocities = spring._startVelocities
		local isMoving = false

		for index, goal in ipairs(spring._springGoals) do
			local oldDisplacement = startDisplacements[index]
			local oldVelocity = startVelocities[index]
			local newDisplacement = oldDisplacement * posPos + oldVelocity * posVel
			local newVelocity = oldDisplacement * velPos + oldVelocity * velVel

			if math.abs(newDisplacement) > EPSILON or math.abs(newVelocity) > EPSILON then
				isMoving = true
			end

			positions[index] = newDisplacement + goal
			velocities[index] = newVelocity
		end

		if not isMoving then
			springsToSleep[spring] = true
		end
	end

	for spring in pairs(activeSprings) do
		spring._currentValue = packType(spring._springPositions, spring._currentType)
		updateAll(spring)
	end

	for spring in pairs(springsToSleep) do
		activeSprings[spring] = nil
	end
end

RunService:BindToRenderStep(
	"__FusionSpringScheduler",
	Enum.RenderPriority.First.Value,
	updateAllSprings
)

return SpringScheduler
end)() end,
    [7] = function()local wax,script,require=ImportGlobals(7)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs a new computed state object, which follows the value of another
	state object using a tween.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Types = require(Package.Types)
local TweenScheduler = require(Package.Animation.TweenScheduler)
local useDependency = require(Package.Dependencies.useDependency)
local initDependency = require(Package.Dependencies.initDependency)
local logError = require(Package.Logging.logError)
local logErrorNonFatal = require(Package.Logging.logErrorNonFatal)
local xtypeof = require(Package.Utility.xtypeof)

local class = {}

local CLASS_METATABLE = {__index = class}
local WEAK_KEYS_METATABLE = {__mode = "k"}

--[[
	Returns the current value of this Tween object.
	The object will be registered as a dependency unless `asDependency` is false.
]]
function class:get(asDependency: boolean?): any
	if asDependency ~= false then
		useDependency(self)
	end
	return self._currentValue
end

--[[
	Called when the goal state changes value; this will initiate a new tween.
	Returns false as the current value doesn't change right away.
]]
function class:update(): boolean
	local goalValue = self._goalState:get(false)

	-- if the goal hasn't changed, then this is a TweenInfo change.
	-- in that case, if we're not currently animating, we can skip everything
	if goalValue == self._nextValue and not self._currentlyAnimating then
		return false
	end

	local tweenInfo = self._tweenInfo
	if self._tweenInfoIsState then
		tweenInfo = tweenInfo:get()
	end

	-- if we receive a bad TweenInfo, then error and stop the update
	if typeof(tweenInfo) ~= "TweenInfo" then
		logErrorNonFatal("mistypedTweenInfo", nil, typeof(tweenInfo))
		return false
	end

	self._prevValue = self._currentValue
	self._nextValue = goalValue

	self._currentTweenStartTime = os.clock()
	self._currentTweenInfo = tweenInfo

	local tweenDuration = tweenInfo.DelayTime + tweenInfo.Time
	if tweenInfo.Reverses then
		tweenDuration += tweenInfo.Time
	end
	tweenDuration *= tweenInfo.RepeatCount + 1
	self._currentTweenDuration = tweenDuration

	-- start animating this tween
	TweenScheduler.add(self)

	return false
end

local function Tween<T>(
	goalState: PubTypes.StateObject<PubTypes.Animatable>,
	tweenInfo: PubTypes.CanBeState<TweenInfo>?
): Types.Tween<T>
	local currentValue = goalState:get(false)

	-- apply defaults for tween info
	if tweenInfo == nil then
		tweenInfo = TweenInfo.new()
	end

	local dependencySet = {[goalState] = true}
	local tweenInfoIsState = xtypeof(tweenInfo) == "State"

	if tweenInfoIsState then
		dependencySet[tweenInfo] = true
	end

	local startingTweenInfo = tweenInfo
	if tweenInfoIsState then
		startingTweenInfo = startingTweenInfo:get()
	end

	-- If we start with a bad TweenInfo, then we don't want to construct a Tween
	if typeof(startingTweenInfo) ~= "TweenInfo" then
		logError("mistypedTweenInfo", nil, typeof(startingTweenInfo))
	end

	local self = setmetatable({
		type = "State",
		kind = "Tween",
		dependencySet = dependencySet,
		-- if we held strong references to the dependents, then they wouldn't be
		-- able to get garbage collected when they fall out of scope
		dependentSet = setmetatable({}, WEAK_KEYS_METATABLE),
		_goalState = goalState,
		_tweenInfo = tweenInfo,
		_tweenInfoIsState = tweenInfoIsState,

		_prevValue = currentValue,
		_nextValue = currentValue,
		_currentValue = currentValue,

		-- store current tween into separately from 'real' tween into, so it
		-- isn't affected by :setTweenInfo() until next change
		_currentTweenInfo = tweenInfo,
		_currentTweenDuration = 0,
		_currentTweenStartTime = 0,
		_currentlyAnimating = false
	}, CLASS_METATABLE)

	initDependency(self)
	-- add this object to the goal state's dependent set
	goalState.dependentSet[self] = true

	return self
end

return Tween
end)() end,
    [8] = function()local wax,script,require=ImportGlobals(8)local ImportGlobals return (function(...)--!strict

--[[
	Manages batch updating of tween objects.
]]

local RunService = game:GetService("RunService")

local Package = script.Parent.Parent
local Types = require(Package.Types)
local lerpType = require(Package.Animation.lerpType)
local getTweenRatio = require(Package.Animation.getTweenRatio)
local updateAll = require(Package.Dependencies.updateAll)

local TweenScheduler = {}

type Set<T> = {[T]: any}
type Tween = Types.Tween<any>

local WEAK_KEYS_METATABLE = {__mode = "k"}

-- all the tweens currently being updated
local allTweens: Set<Tween> = {}
setmetatable(allTweens, WEAK_KEYS_METATABLE)

--[[
	Adds a Tween to be updated every render step.
]]
function TweenScheduler.add(tween: Tween)
	allTweens[tween] = true
end

--[[
	Removes a Tween from the scheduler.
]]
function TweenScheduler.remove(tween: Tween)
	allTweens[tween] = nil
end

--[[
	Updates all Tween objects.
]]
local function updateAllTweens()
	local now = os.clock()
	-- FIXME: Typed Luau doesn't understand this loop yet
	for tween: Tween in pairs(allTweens :: any) do
		local currentTime = now - tween._currentTweenStartTime

		if currentTime > tween._currentTweenDuration then
			if tween._currentTweenInfo.Reverses then
				tween._currentValue = tween._prevValue
			else
				tween._currentValue = tween._nextValue
			end
			tween._currentlyAnimating = false
			updateAll(tween)
			TweenScheduler.remove(tween)
		else
			local ratio = getTweenRatio(tween._currentTweenInfo, currentTime)
			local currentValue = lerpType(tween._prevValue, tween._nextValue, ratio)
			tween._currentValue = currentValue
			tween._currentlyAnimating = true
			updateAll(tween)
		end
	end
end

RunService:BindToRenderStep(
	"__FusionTweenScheduler",
	Enum.RenderPriority.First.Value,
	updateAllTweens
)

return TweenScheduler
end)() end,
    [9] = function()local wax,script,require=ImportGlobals(9)local ImportGlobals return (function(...)--!strict

--[[
	Given a `tweenInfo` and `currentTime`, returns a ratio which can be used to
	tween between two values over time.
]]

local TweenService = game:GetService("TweenService")

local function getTweenRatio(tweenInfo: TweenInfo, currentTime: number): number
	local delay = tweenInfo.DelayTime
	local duration = tweenInfo.Time
	local reverses = tweenInfo.Reverses
	local numCycles = 1 + tweenInfo.RepeatCount
	local easeStyle = tweenInfo.EasingStyle
	local easeDirection = tweenInfo.EasingDirection

	local cycleDuration = delay + duration
	if reverses then
		cycleDuration += duration
	end

	if currentTime >= cycleDuration * numCycles then
		return 1
	end

	local cycleTime = currentTime % cycleDuration

	if cycleTime <= delay then
		return 0
	end

	local tweenProgress = (cycleTime - delay) / duration
	if tweenProgress > 1 then
		tweenProgress = 2 - tweenProgress
	end

	local ratio = TweenService:GetValue(tweenProgress, easeStyle, easeDirection)
	return ratio
end

return getTweenRatio
end)() end,
    [10] = function()local wax,script,require=ImportGlobals(10)local ImportGlobals return (function(...)--!strict

--[[
	Linearly interpolates the given animatable types by a ratio.
	If the types are different or not animatable, then the first value will be
	returned for ratios below 0.5, and the second value for 0.5 and above.

	FIXME: This function uses a lot of redefinitions to suppress false positives
	from the Luau typechecker - ideally these wouldn't be required
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Oklab = require(Package.Colour.Oklab)

local function lerpType(from: any, to: any, ratio: number): any
	local typeString = typeof(from)

	if typeof(to) == typeString then
		-- both types must match for interpolation to make sense
		if typeString == "number" then
			local to, from = to :: number, from :: number
			return (to - from) * ratio + from

		elseif typeString == "CFrame" then
			local to, from = to :: CFrame, from :: CFrame
			return from:Lerp(to, ratio)

		elseif typeString == "Color3" then
			local to, from = to :: Color3, from :: Color3
			local fromLab = Oklab.to(from)
			local toLab = Oklab.to(to)
			return Oklab.from(
				fromLab:Lerp(toLab, ratio),
				false
			)

		elseif typeString == "ColorSequenceKeypoint" then
			local to, from = to :: ColorSequenceKeypoint, from :: ColorSequenceKeypoint
			local fromLab = Oklab.to(from.Value)
			local toLab = Oklab.to(to.Value)
			return ColorSequenceKeypoint.new(
				(to.Time - from.Time) * ratio + from.Time,
				Oklab.from(
					fromLab:Lerp(toLab, ratio),
					false
				)
			)

		elseif typeString == "DateTime" then
			local to, from = to :: DateTime, from :: DateTime
			return DateTime.fromUnixTimestampMillis(
				(to.UnixTimestampMillis - from.UnixTimestampMillis) * ratio + from.UnixTimestampMillis
			)

		elseif typeString == "NumberRange" then
			local to, from = to :: NumberRange, from :: NumberRange
			return NumberRange.new(
				(to.Min - from.Min) * ratio + from.Min,
				(to.Max - from.Max) * ratio + from.Max
			)

		elseif typeString == "NumberSequenceKeypoint" then
			local to, from = to :: NumberSequenceKeypoint, from :: NumberSequenceKeypoint
			return NumberSequenceKeypoint.new(
				(to.Time - from.Time) * ratio + from.Time,
				(to.Value - from.Value) * ratio + from.Value,
				(to.Envelope - from.Envelope) * ratio + from.Envelope
			)

		elseif typeString == "PhysicalProperties" then
			local to, from = to :: PhysicalProperties, from :: PhysicalProperties
			return PhysicalProperties.new(
				(to.Density - from.Density) * ratio + from.Density,
				(to.Friction - from.Friction) * ratio + from.Friction,
				(to.Elasticity - from.Elasticity) * ratio + from.Elasticity,
				(to.FrictionWeight - from.FrictionWeight) * ratio + from.FrictionWeight,
				(to.ElasticityWeight - from.ElasticityWeight) * ratio + from.ElasticityWeight
			)

		elseif typeString == "Ray" then
			local to, from = to :: Ray, from :: Ray
			return Ray.new(
				from.Origin:Lerp(to.Origin, ratio),
				from.Direction:Lerp(to.Direction, ratio)
			)

		elseif typeString == "Rect" then
			local to, from = to :: Rect, from :: Rect
			return Rect.new(
				from.Min:Lerp(to.Min, ratio),
				from.Max:Lerp(to.Max, ratio)
			)

		elseif typeString == "Region3" then
			local to, from = to :: Region3, from :: Region3
			-- FUTURE: support rotated Region3s if/when they become constructable
			local position = from.CFrame.Position:Lerp(to.CFrame.Position, ratio)
			local halfSize = from.Size:Lerp(to.Size, ratio) / 2
			return Region3.new(position - halfSize, position + halfSize)

		elseif typeString == "Region3int16" then
			local to, from = to :: Region3int16, from :: Region3int16
			return Region3int16.new(
				Vector3int16.new(
					(to.Min.X - from.Min.X) * ratio + from.Min.X,
					(to.Min.Y - from.Min.Y) * ratio + from.Min.Y,
					(to.Min.Z - from.Min.Z) * ratio + from.Min.Z
				),
				Vector3int16.new(
					(to.Max.X - from.Max.X) * ratio + from.Max.X,
					(to.Max.Y - from.Max.Y) * ratio + from.Max.Y,
					(to.Max.Z - from.Max.Z) * ratio + from.Max.Z
				)
			)

		elseif typeString == "UDim" then
			local to, from = to :: UDim, from :: UDim
			return UDim.new(
				(to.Scale - from.Scale) * ratio + from.Scale,
				(to.Offset - from.Offset) * ratio + from.Offset
			)

		elseif typeString == "UDim2" then
			local to, from = to :: UDim2, from :: UDim2
			return from:Lerp(to, ratio)

		elseif typeString == "Vector2" then
			local to, from = to :: Vector2, from :: Vector2
			return from:Lerp(to, ratio)

		elseif typeString == "Vector2int16" then
			local to, from = to :: Vector2int16, from :: Vector2int16
			return Vector2int16.new(
				(to.X - from.X) * ratio + from.X,
				(to.Y - from.Y) * ratio + from.Y
			)

		elseif typeString == "Vector3" then
			local to, from = to :: Vector3, from :: Vector3
			return from:Lerp(to, ratio)

		elseif typeString == "Vector3int16" then
			local to, from = to :: Vector3int16, from :: Vector3int16
			return Vector3int16.new(
				(to.X - from.X) * ratio + from.X,
				(to.Y - from.Y) * ratio + from.Y,
				(to.Z - from.Z) * ratio + from.Z
			)
		end
	end

	-- fallback case: the types are different or not animatable
	if ratio < 0.5 then
		return from
	else
		return to
	end
end

return lerpType
end)() end,
    [11] = function()local wax,script,require=ImportGlobals(11)local ImportGlobals return (function(...)--!strict

--[[
	Packs an array of numbers into a given animatable data type.
	If the type is not animatable, nil will be returned.

	FUTURE: When Luau supports singleton types, those could be used in
	conjunction with intersection types to make this function fully statically
	type checkable.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Oklab = require(Package.Colour.Oklab)

local function packType(numbers: {number}, typeString: string): PubTypes.Animatable?
	if typeString == "number" then
		return numbers[1]

	elseif typeString == "CFrame" then
		return
			CFrame.new(numbers[1], numbers[2], numbers[3]) *
			CFrame.fromAxisAngle(
				Vector3.new(numbers[4], numbers[5], numbers[6]).Unit,
				numbers[7]
			)

	elseif typeString == "Color3" then
		return Oklab.from(
			Vector3.new(numbers[1], numbers[2], numbers[3]),
			false
		)

	elseif typeString == "ColorSequenceKeypoint" then
		return ColorSequenceKeypoint.new(
			numbers[4],
			Oklab.from(
				Vector3.new(numbers[1], numbers[2], numbers[3]),
				false
			)
		)

	elseif typeString == "DateTime" then
		return DateTime.fromUnixTimestampMillis(numbers[1])

	elseif typeString == "NumberRange" then
		return NumberRange.new(numbers[1], numbers[2])

	elseif typeString == "NumberSequenceKeypoint" then
		return NumberSequenceKeypoint.new(numbers[2], numbers[1], numbers[3])

	elseif typeString == "PhysicalProperties" then
		return PhysicalProperties.new(numbers[1], numbers[2], numbers[3], numbers[4], numbers[5])

	elseif typeString == "Ray" then
		return Ray.new(
			Vector3.new(numbers[1], numbers[2], numbers[3]),
			Vector3.new(numbers[4], numbers[5], numbers[6])
		)

	elseif typeString == "Rect" then
		return Rect.new(numbers[1], numbers[2], numbers[3], numbers[4])

	elseif typeString == "Region3" then
		-- FUTURE: support rotated Region3s if/when they become constructable
		local position = Vector3.new(numbers[1], numbers[2], numbers[3])
		local halfSize = Vector3.new(numbers[4] / 2, numbers[5] / 2, numbers[6] / 2)
		return Region3.new(position - halfSize, position + halfSize)

	elseif typeString == "Region3int16" then
		return Region3int16.new(
			Vector3int16.new(numbers[1], numbers[2], numbers[3]),
			Vector3int16.new(numbers[4], numbers[5], numbers[6])
		)

	elseif typeString == "UDim" then
		return UDim.new(numbers[1], numbers[2])

	elseif typeString == "UDim2" then
		return UDim2.new(numbers[1], numbers[2], numbers[3], numbers[4])

	elseif typeString == "Vector2" then
		return Vector2.new(numbers[1], numbers[2])

	elseif typeString == "Vector2int16" then
		return Vector2int16.new(numbers[1], numbers[2])

	elseif typeString == "Vector3" then
		return Vector3.new(numbers[1], numbers[2], numbers[3])

	elseif typeString == "Vector3int16" then
		return Vector3int16.new(numbers[1], numbers[2], numbers[3])
	else
		return nil
	end
end

return packType
end)() end,
    [12] = function()local wax,script,require=ImportGlobals(12)local ImportGlobals return (function(...)--!strict

--[[
	Returns a 2x2 matrix of coefficients for a given time, damping and speed.
	Specifically, this returns four coefficients - posPos, posVel, velPos, and
	velVel - which can be multiplied with position and velocity like so:

	local newPosition = oldPosition * posPos + oldVelocity * posVel
	local newVelocity = oldPosition * velPos + oldVelocity * velVel

	Special thanks to AxisAngle for helping to improve numerical precision.
]]

local function springCoefficients(time: number, damping: number, speed: number): (number, number, number, number)
	-- if time or speed is 0, then the spring won't move
	if time == 0 or speed == 0 then
		return 1, 0, 0, 1
	end
	local posPos, posVel, velPos, velVel

	if damping > 1 then
		-- overdamped spring
		-- solution to the characteristic equation:
		-- z = -ζω ± Sqrt[ζ^2 - 1] ω
		-- x[t] -> x0(e^(t z2) z1 - e^(t z1) z2)/(z1 - z2)
		--		 + v0(e^(t z1) - e^(t z2))/(z1 - z2)
		-- v[t] -> x0(z1 z2(-e^(t z1) + e^(t z2)))/(z1 - z2)
		--		 + v0(z1 e^(t z1) - z2 e^(t z2))/(z1 - z2)

		local scaledTime = time * speed
		local alpha = math.sqrt(damping^2 - 1)
		local scaledInvAlpha = -0.5 / alpha
		local z1 = -alpha - damping
		local z2 = 1 / z1
		local expZ1 = math.exp(scaledTime * z1)
		local expZ2 = math.exp(scaledTime * z2)

		posPos = (expZ2*z1 - expZ1*z2) * scaledInvAlpha
		posVel = (expZ1 - expZ2) * scaledInvAlpha / speed
		velPos = (expZ2 - expZ1) * scaledInvAlpha * speed
		velVel = (expZ1*z1 - expZ2*z2) * scaledInvAlpha

	elseif damping == 1 then
		-- critically damped spring
		-- x[t] -> x0(e^-tω)(1+tω) + v0(e^-tω)t
		-- v[t] -> x0(t ω^2)(-e^-tω) + v0(1 - tω)(e^-tω)

		local scaledTime = time * speed
		local expTerm = math.exp(-scaledTime)

		posPos = expTerm * (1 + scaledTime)
		posVel = expTerm * time
		velPos = expTerm * (-scaledTime*speed)
		velVel = expTerm * (1 - scaledTime)

	else
		-- underdamped spring
		-- factored out of the solutions to the characteristic equation:
		-- α = Sqrt[1 - ζ^2]
		-- x[t] -> x0(e^-tζω)(α Cos[tα] + ζω Sin[tα])/α
		--       + v0(e^-tζω)(Sin[tα])/α
		-- v[t] -> x0(-e^-tζω)(α^2 + ζ^2 ω^2)(Sin[tα])/α
		--       + v0(e^-tζω)(α Cos[tα] - ζω Sin[tα])/α

		local scaledTime = time * speed
		local alpha = math.sqrt(1 - damping^2)
		local invAlpha = 1 / alpha
		local alphaTime = alpha * scaledTime
		local expTerm = math.exp(-scaledTime*damping)
		local sinTerm = expTerm * math.sin(alphaTime)
		local cosTerm = expTerm * math.cos(alphaTime)
		local sinInvAlpha = sinTerm*invAlpha
		local sinInvAlphaDamp = sinInvAlpha*damping

		posPos = sinInvAlphaDamp + cosTerm
		posVel = sinInvAlpha
		velPos = -(sinInvAlphaDamp*damping + sinTerm*alpha)
		velVel = cosTerm - sinInvAlphaDamp
	end

	return posPos, posVel, velPos, velVel
end

return springCoefficients

end)() end,
    [13] = function()local wax,script,require=ImportGlobals(13)local ImportGlobals return (function(...)--!strict

--[[
	Unpacks an animatable type into an array of numbers.
	If the type is not animatable, an empty array will be returned.

	FIXME: This function uses a lot of redefinitions to suppress false positives
	from the Luau typechecker - ideally these wouldn't be required

	FUTURE: When Luau supports singleton types, those could be used in
	conjunction with intersection types to make this function fully statically
	type checkable.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Oklab = require(Package.Colour.Oklab)

local function unpackType(value: any, typeString: string): {number}
	if typeString == "number" then
		local value = value :: number
		return {value}

	elseif typeString == "CFrame" then
		-- FUTURE: is there a better way of doing this? doing distance
		-- calculations on `angle` may be incorrect
		local axis, angle = value:ToAxisAngle()
		return {value.X, value.Y, value.Z, axis.X, axis.Y, axis.Z, angle}

	elseif typeString == "Color3" then
		local lab = Oklab.to(value)
		return {lab.X, lab.Y, lab.Z}

	elseif typeString == "ColorSequenceKeypoint" then
		local lab = Oklab.to(value.Value)
		return {lab.X, lab.Y, lab.Z, value.Time}

	elseif typeString == "DateTime" then
		return {value.UnixTimestampMillis}

	elseif typeString == "NumberRange" then
		return {value.Min, value.Max}

	elseif typeString == "NumberSequenceKeypoint" then
		return {value.Value, value.Time, value.Envelope}

	elseif typeString == "PhysicalProperties" then
		return {value.Density, value.Friction, value.Elasticity, value.FrictionWeight, value.ElasticityWeight}

	elseif typeString == "Ray" then
		return {value.Origin.X, value.Origin.Y, value.Origin.Z, value.Direction.X, value.Direction.Y, value.Direction.Z}

	elseif typeString == "Rect" then
		return {value.Min.X, value.Min.Y, value.Max.X, value.Max.Y}

	elseif typeString == "Region3" then
		-- FUTURE: support rotated Region3s if/when they become constructable
		return {
			value.CFrame.X, value.CFrame.Y, value.CFrame.Z,
			value.Size.X, value.Size.Y, value.Size.Z
		}

	elseif typeString == "Region3int16" then
		return {value.Min.X, value.Min.Y, value.Min.Z, value.Max.X, value.Max.Y, value.Max.Z}

	elseif typeString == "UDim" then
		return {value.Scale, value.Offset}

	elseif typeString == "UDim2" then
		return {value.X.Scale, value.X.Offset, value.Y.Scale, value.Y.Offset}

	elseif typeString == "Vector2" then
		return {value.X, value.Y}

	elseif typeString == "Vector2int16" then
		return {value.X, value.Y}

	elseif typeString == "Vector3" then
		return {value.X, value.Y, value.Z}

	elseif typeString == "Vector3int16" then
		return {value.X, value.Y, value.Z}
	else
		return {}
	end
end

return unpackType
end)() end,
    [15] = function()local wax,script,require=ImportGlobals(15)local ImportGlobals return (function(...)--!strict

--[[
	Provides functions for converting Color3s into Oklab space, for more
	perceptually uniform colour blending.

	See: https://bottosson.github.io/posts/oklab/
]]

local Oklab = {}

-- Converts a Color3 in RGB space to a Vector3 in Oklab space.
function Oklab.to(rgb: Color3): Vector3
	local l = rgb.R * 0.4122214708 + rgb.G * 0.5363325363 + rgb.B * 0.0514459929
	local m = rgb.R * 0.2119034982 + rgb.G * 0.6806995451 + rgb.B * 0.1073969566
	local s = rgb.R * 0.0883024619 + rgb.G * 0.2817188376 + rgb.B * 0.6299787005

	local lRoot = l ^ (1/3)
	local mRoot = m ^ (1/3)
	local sRoot = s ^ (1/3)

	return Vector3.new(
		lRoot * 0.2104542553 + mRoot * 0.7936177850 - sRoot * 0.0040720468,
		lRoot * 1.9779984951 - mRoot * 2.4285922050 + sRoot * 0.4505937099,
		lRoot * 0.0259040371 + mRoot * 0.7827717662 - sRoot * 0.8086757660
	)
end

-- Converts a Vector3 in CIELAB space to a Color3 in RGB space.
-- The Color3 will be clamped by default unless specified otherwise.
function Oklab.from(lab: Vector3, unclamped: boolean?): Color3
	local lRoot = lab.X + lab.Y * 0.3963377774 + lab.Z * 0.2158037573
	local mRoot = lab.X - lab.Y * 0.1055613458 - lab.Z * 0.0638541728
	local sRoot = lab.X - lab.Y * 0.0894841775 - lab.Z * 1.2914855480

	local l = lRoot ^ 3
	local m = mRoot ^ 3
	local s = sRoot ^ 3

	local red = l * 4.0767416621 - m * 3.3077115913 + s * 0.2309699292
	local green = l * -1.2684380046 + m * 2.6097574011 - s * 0.3413193965
	local blue = l * -0.0041960863 - m * 0.7034186147 + s * 1.7076147010

	if not unclamped then
		red = math.clamp(red, 0, 1)
		green = math.clamp(green, 0, 1)
		blue = math.clamp(blue, 0, 1)
	end

	return Color3.new(red, green, blue)
end

return Oklab

end)() end,
    [17] = function()local wax,script,require=ImportGlobals(17)local ImportGlobals return (function(...)--!strict

--[[
	Calls the given callback, and stores any used external dependencies.
	Arguments can be passed in after the callback.
	If the callback completed successfully, returns true and the returned value,
	otherwise returns false and the error thrown.
	The callback shouldn't yield or run asynchronously.

	NOTE: any calls to useDependency() inside the callback (even if inside any
	nested captureDependencies() call) will not be included in the set, to avoid
	self-dependencies.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local parseError = require(Package.Logging.parseError)
local sharedState = require(Package.Dependencies.sharedState)

type Set<T> = {[T]: any}

local initialisedStack = sharedState.initialisedStack
local initialisedStackCapacity = 0

local function captureDependencies(
	saveToSet: Set<PubTypes.Dependency>,
	callback: (...any) -> any,
	...
): (boolean, any)

	local prevDependencySet = sharedState.dependencySet
	sharedState.dependencySet = saveToSet

	sharedState.initialisedStackSize += 1
	local initialisedStackSize = sharedState.initialisedStackSize

	local initialisedSet
	if initialisedStackSize > initialisedStackCapacity then
		initialisedSet = {}
		initialisedStack[initialisedStackSize] = initialisedSet
		initialisedStackCapacity = initialisedStackSize
	else
		initialisedSet = initialisedStack[initialisedStackSize]
		table.clear(initialisedSet)
	end

	local data = table.pack(xpcall(callback, parseError, ...))

	sharedState.dependencySet = prevDependencySet
	sharedState.initialisedStackSize -= 1

	return table.unpack(data, 1, data.n)
end

return captureDependencies

end)() end,
    [18] = function()local wax,script,require=ImportGlobals(18)local ImportGlobals return (function(...)--!strict

--[[
	Registers the creation of an object which can be used as a dependency.

	This is used to make sure objects don't capture dependencies originating
	from inside of themselves.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local sharedState = require(Package.Dependencies.sharedState)

local initialisedStack = sharedState.initialisedStack

local function initDependency(dependency: PubTypes.Dependency)
	local initialisedStackSize = sharedState.initialisedStackSize

	for index, initialisedSet in ipairs(initialisedStack) do
		if index > initialisedStackSize then
			return
		end

		initialisedSet[dependency] = true
	end
end

return initDependency
end)() end,
    [19] = function()local wax,script,require=ImportGlobals(19)local ImportGlobals return (function(...)--!strict

--[[
	Stores shared state for dependency management functions.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)

type Set<T> = {[T]: any}

-- The set where used dependencies should be saved to.
local dependencySet: Set<PubTypes.Dependency>? = nil

-- A stack of sets where newly created dependencies should be stored.
local initialisedStack: {Set<PubTypes.Dependency>} = {}
local initialisedStackSize = 0

return {
	dependencySet = dependencySet,
	initialisedStack = initialisedStack,
	initialisedStackSize = initialisedStackSize
}
end)() end,
    [20] = function()local wax,script,require=ImportGlobals(20)local ImportGlobals return (function(...)--!strict

--[[
	Given a reactive object, updates all dependent reactive objects.
	Objects are only ever updated after all of their dependencies are updated,
	are only ever updated once, and won't be updated if their dependencies are
	unchanged.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)

type Set<T> = {[T]: any}
type Descendant = (PubTypes.Dependent & PubTypes.Dependency) | PubTypes.Dependent

-- Credit: https://blog.elttob.uk/2022/11/07/sets-efficient-topological-search.html
local function updateAll(root: PubTypes.Dependency)
	local counters: {[Descendant]: number} = {}
	local flags: {[Descendant]: boolean} = {}
	local queue: {Descendant} = {}
	local queueSize = 0
	local queuePos = 1

	for object in root.dependentSet do
		queueSize += 1
		queue[queueSize] = object
		flags[object] = true
	end

	-- Pass 1: counting up
	while queuePos <= queueSize do
		local next = queue[queuePos]
		local counter = counters[next]
		counters[next] = if counter == nil then 1 else counter + 1
		if (next :: any).dependentSet ~= nil then
			for object in (next :: any).dependentSet do
				queueSize += 1
				queue[queueSize] = object
			end
		end
		queuePos += 1
	end

	-- Pass 2: counting down + processing
	queuePos = 1
	while queuePos <= queueSize do
		local next = queue[queuePos]
		local counter = counters[next] - 1
		counters[next] = counter
		if counter == 0 and flags[next] and next:update() and (next :: any).dependentSet ~= nil then
			for object in (next :: any).dependentSet do
				flags[object] = true
			end
		end
		queuePos += 1
	end
end

return updateAll
end)() end,
    [21] = function()local wax,script,require=ImportGlobals(21)local ImportGlobals return (function(...)--!strict

--[[
	If a target set was specified by captureDependencies(), this will add the
	given dependency to the target set.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local sharedState = require(Package.Dependencies.sharedState)

local initialisedStack = sharedState.initialisedStack

local function useDependency(dependency: PubTypes.Dependency)
	local dependencySet = sharedState.dependencySet

	if dependencySet ~= nil then
		local initialisedStackSize = sharedState.initialisedStackSize
		if initialisedStackSize > 0 then
			local initialisedSet = initialisedStack[initialisedStackSize]
			if initialisedSet[dependency] ~= nil then
				return
			end
		end
		dependencySet[dependency] = true
	end
end

return useDependency
end)() end,
    [23] = function()local wax,script,require=ImportGlobals(23)local ImportGlobals return (function(...)--!strict

--[[
	A special key for property tables, which parents any given descendants into
	an instance.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local logWarn = require(Package.Logging.logWarn)
local Observer = require(Package.State.Observer)
local xtypeof = require(Package.Utility.xtypeof)

type Set<T> = {[T]: boolean}

-- Experimental flag: name children based on the key used in the [Children] table
local EXPERIMENTAL_AUTO_NAMING = false

local Children = {}
Children.type = "SpecialKey"
Children.kind = "Children"
Children.stage = "descendants"

function Children:apply(propValue: any, applyTo: Instance, cleanupTasks: {PubTypes.Task})
	local newParented: Set<Instance> = {}
	local oldParented: Set<Instance> = {}

	-- save disconnection functions for state object observers
	local newDisconnects: {[PubTypes.StateObject<any>]: () -> ()} = {}
	local oldDisconnects: {[PubTypes.StateObject<any>]: () -> ()} = {}

	local updateQueued = false
	local queueUpdate: () -> ()

	-- Rescans this key's value to find new instances to parent and state objects
	-- to observe for changes; then unparents instances no longer found and
	-- disconnects observers for state objects no longer present.
	local function updateChildren()
		if not updateQueued then
			return -- this update may have been canceled by destruction, etc.
		end
		updateQueued = false

		oldParented, newParented = newParented, oldParented
		oldDisconnects, newDisconnects = newDisconnects, oldDisconnects
		table.clear(newParented)
		table.clear(newDisconnects)

		local function processChild(child: any, autoName: string?)
			local kind = xtypeof(child)

			if kind == "Instance" then
				-- case 1; single instance

				newParented[child] = true
				if oldParented[child] == nil then
					-- wasn't previously present

					-- TODO: check for ancestry conflicts here
					child.Parent = applyTo
				else
					-- previously here; we want to reuse, so remove from old
					-- set so we don't encounter it during unparenting
					oldParented[child] = nil
				end

				if EXPERIMENTAL_AUTO_NAMING and autoName ~= nil then
					child.Name = autoName
				end

			elseif kind == "State" then
				-- case 2; state object

				local value = child:get(false)
				-- allow nil to represent the absence of a child
				if value ~= nil then
					processChild(value, autoName)
				end

				local disconnect = oldDisconnects[child]
				if disconnect == nil then
					-- wasn't previously present
					disconnect = Observer(child):onChange(queueUpdate)
				else
					-- previously here; we want to reuse, so remove from old
					-- set so we don't encounter it during unparenting
					oldDisconnects[child] = nil
				end

				newDisconnects[child] = disconnect

			elseif kind == "table" then
				-- case 3; table of objects

				for key, subChild in pairs(child) do
					local keyType = typeof(key)
					local subAutoName: string? = nil

					if keyType == "string" then
						subAutoName = key
					elseif keyType == "number" and autoName ~= nil then
						subAutoName = autoName .. "_" .. key
					end

					processChild(subChild, subAutoName)
				end

			else
				logWarn("unrecognisedChildType", kind)
			end
		end

		if propValue ~= nil then
			-- `propValue` is set to nil on cleanup, so we don't process children
			-- in that case
			processChild(propValue)
		end

		-- unparent any children that are no longer present
		for oldInstance in pairs(oldParented) do
			oldInstance.Parent = nil
		end

		-- disconnect observers which weren't reused
		for oldState, disconnect in pairs(oldDisconnects) do
			disconnect()
		end
	end

	queueUpdate = function()
		if not updateQueued then
			updateQueued = true
			task.defer(updateChildren)
		end
	end

	table.insert(cleanupTasks, function()
		propValue = nil
		updateQueued = true
		updateChildren()
	end)

	-- perform initial child parenting
	updateQueued = true
	updateChildren()
end

return Children :: PubTypes.SpecialKey
end)() end,
    [24] = function()local wax,script,require=ImportGlobals(24)local ImportGlobals return (function(...)--!strict

--[[
	A special key for property tables, which adds user-specified tasks to be run
	when the instance is destroyed.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)

local Cleanup = {}
Cleanup.type = "SpecialKey"
Cleanup.kind = "Cleanup"
Cleanup.stage = "observer"

function Cleanup:apply(userTask: any, applyTo: Instance, cleanupTasks: {PubTypes.Task})
	table.insert(cleanupTasks, userTask)
end

return Cleanup
end)() end,
    [25] = function()local wax,script,require=ImportGlobals(25)local ImportGlobals return (function(...)--!strict

--[[
	Processes and returns an existing instance, with options for setting
	properties, event handlers and other attributes on the instance.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local applyInstanceProps = require(Package.Instances.applyInstanceProps)

local function Hydrate(target: Instance)
	return function(props: PubTypes.PropertyTable): Instance
		applyInstanceProps(props, target)
		return target
	end
end

return Hydrate
end)() end,
    [26] = function()local wax,script,require=ImportGlobals(26)local ImportGlobals return (function(...)--!strict

--[[
	Constructs and returns a new instance, with options for setting properties,
	event handlers and other attributes on the instance right away.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local defaultProps = require(Package.Instances.defaultProps)
local applyInstanceProps = require(Package.Instances.applyInstanceProps)
local logError= require(Package.Logging.logError)

local function New(className: string)
	return function(props: PubTypes.PropertyTable): Instance
		local ok, instance = pcall(Instance.new, className)

		if not ok then
			logError("cannotCreateClass", nil, className)
		end

		local classDefaults = defaultProps[className]
		if classDefaults ~= nil then
			for defaultProp, defaultValue in pairs(classDefaults) do
				instance[defaultProp] = defaultValue
			end
		end

		applyInstanceProps(props, instance)

		return instance
	end
end

return New
end)() end,
    [27] = function()local wax,script,require=ImportGlobals(27)local ImportGlobals return (function(...)--!strict

--[[
	Constructs special keys for property tables which connect property change
	listeners to an instance.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local logError = require(Package.Logging.logError)

local function OnChange(propertyName: string): PubTypes.SpecialKey
	local changeKey = {}
	changeKey.type = "SpecialKey"
	changeKey.kind = "OnChange"
	changeKey.stage = "observer"

	function changeKey:apply(callback: any, applyTo: Instance, cleanupTasks: {PubTypes.Task})
		local ok, event = pcall(applyTo.GetPropertyChangedSignal, applyTo, propertyName)
		if not ok then
			logError("cannotConnectChange", nil, applyTo.ClassName, propertyName)
		elseif typeof(callback) ~= "function" then
			logError("invalidChangeHandler", nil, propertyName)
		else
			table.insert(cleanupTasks, event:Connect(function()
				callback((applyTo :: any)[propertyName])
			end))
		end
	end

	return changeKey
end

return OnChange
end)() end,
    [28] = function()local wax,script,require=ImportGlobals(28)local ImportGlobals return (function(...)--!strict

--[[
	Constructs special keys for property tables which connect event listeners to
	an instance.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local logError = require(Package.Logging.logError)

local function getProperty_unsafe(instance: Instance, property: string)
	return (instance :: any)[property]
end

local function OnEvent(eventName: string): PubTypes.SpecialKey
	local eventKey = {}
	eventKey.type = "SpecialKey"
	eventKey.kind = "OnEvent"
	eventKey.stage = "observer"

	function eventKey:apply(callback: any, applyTo: Instance, cleanupTasks: {PubTypes.Task})
		local ok, event = pcall(getProperty_unsafe, applyTo, eventName)
		if not ok or typeof(event) ~= "RBXScriptSignal" then
			logError("cannotConnectEvent", nil, applyTo.ClassName, eventName)
		elseif typeof(callback) ~= "function" then
			logError("invalidEventHandler", nil, eventName)
		else
			table.insert(cleanupTasks, event:Connect(callback))
		end
	end

	return eventKey
end

return OnEvent
end)() end,
    [29] = function()local wax,script,require=ImportGlobals(29)local ImportGlobals return (function(...)--!strict

--[[
	A special key for property tables, which allows users to extract values from
	an instance into an automatically-updated Value object.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local logError = require(Package.Logging.logError)
local xtypeof = require(Package.Utility.xtypeof)

local function Out(propertyName: string): PubTypes.SpecialKey
	local outKey = {}
	outKey.type = "SpecialKey"
	outKey.kind = "Out"
	outKey.stage = "observer"

	function outKey:apply(outState: any, applyTo: Instance, cleanupTasks: { PubTypes.Task })
		local ok, event = pcall(applyTo.GetPropertyChangedSignal, applyTo, propertyName)
		if not ok then
			logError("invalidOutProperty", nil, applyTo.ClassName, propertyName)
		elseif xtypeof(outState) ~= "State" or outState.kind ~= "Value" then
			logError("invalidOutType")
		else
			outState:set((applyTo :: any)[propertyName])
			table.insert(
				cleanupTasks,
				event:Connect(function()
					outState:set((applyTo :: any)[propertyName])
				end)
			)
			table.insert(cleanupTasks, function()
				outState:set(nil)
			end)
		end
	end

	return outKey
end

return Out

end)() end,
    [30] = function()local wax,script,require=ImportGlobals(30)local ImportGlobals return (function(...)--!strict

--[[
	A special key for property tables, which stores a reference to the instance
	in a user-provided Value object.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local logError = require(Package.Logging.logError)
local xtypeof = require(Package.Utility.xtypeof)

local Ref = {}
Ref.type = "SpecialKey"
Ref.kind = "Ref"
Ref.stage = "observer"

function Ref:apply(refState: any, applyTo: Instance, cleanupTasks: {PubTypes.Task})
	if xtypeof(refState) ~= "State" or refState.kind ~= "Value" then
		logError("invalidRefType")
	else
		refState:set(applyTo)
		table.insert(cleanupTasks, function()
			refState:set(nil)
		end)
	end
end

return Ref
end)() end,
    [31] = function()local wax,script,require=ImportGlobals(31)local ImportGlobals return (function(...)--!strict

--[[
	Applies a table of properties to an instance, including binding to any
	given state objects and applying any special keys.

	No strong reference is kept by default - special keys should take care not
	to accidentally hold strong references to instances forever.

	If a key is used twice, an error will be thrown. This is done to avoid
	double assignments or double bindings. However, some special keys may want
	to enable such assignments - in which case unique keys should be used for
	each occurence.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local cleanup = require(Package.Utility.cleanup)
local xtypeof = require(Package.Utility.xtypeof)
local logError = require(Package.Logging.logError)
local Observer = require(Package.State.Observer)

local function setProperty_unsafe(instance: Instance, property: string, value: any)
	(instance :: any)[property] = value
end

local function testPropertyAssignable(instance: Instance, property: string)
	(instance :: any)[property] = (instance :: any)[property]
end

local function setProperty(instance: Instance, property: string, value: any)
	if not pcall(setProperty_unsafe, instance, property, value) then
		if not pcall(testPropertyAssignable, instance, property) then
			if instance == nil then
				-- reference has been lost
				logError("setPropertyNilRef", nil, property, tostring(value))
			else
				-- property is not assignable
				logError("cannotAssignProperty", nil, instance.ClassName, property)
			end
		else
			-- property is assignable, but this specific assignment failed
			-- this typically implies the wrong type was received
			local givenType = typeof(value)
			local expectedType = typeof((instance :: any)[property])
			logError("invalidPropertyType", nil, instance.ClassName, property, expectedType, givenType)
		end
	end
end

local function bindProperty(instance: Instance, property: string, value: PubTypes.CanBeState<any>, cleanupTasks: {PubTypes.Task})
	if xtypeof(value) == "State" then
		-- value is a state object - assign and observe for changes
		local willUpdate = false
		local function updateLater()
			if not willUpdate then
				willUpdate = true
				task.defer(function()
					willUpdate = false
					setProperty(instance, property, value:get(false))
				end)
			end
		end

		setProperty(instance, property, value:get(false))
		table.insert(cleanupTasks, Observer(value :: any):onChange(updateLater))
	else
		-- value is a constant - assign once only
		setProperty(instance, property, value)
	end
end

local function applyInstanceProps(props: PubTypes.PropertyTable, applyTo: Instance)
	local specialKeys = {
		self = {} :: {[PubTypes.SpecialKey]: any},
		descendants = {} :: {[PubTypes.SpecialKey]: any},
		ancestor = {} :: {[PubTypes.SpecialKey]: any},
		observer = {} :: {[PubTypes.SpecialKey]: any}
	}
	local cleanupTasks = {}

	for key, value in pairs(props) do
		local keyType = xtypeof(key)

		if keyType == "string" then
			if key ~= "Parent" then
				bindProperty(applyTo, key :: string, value, cleanupTasks)
			end
		elseif keyType == "SpecialKey" then
			local stage = (key :: PubTypes.SpecialKey).stage
			local keys = specialKeys[stage]
			if keys == nil then
				logError("unrecognisedPropertyStage", nil, stage)
			else
				keys[key] = value
			end
		else
			-- we don't recognise what this key is supposed to be
			logError("unrecognisedPropertyKey", nil, xtypeof(key))
		end
	end

	for key, value in pairs(specialKeys.self) do
		key:apply(value, applyTo, cleanupTasks)
	end
	for key, value in pairs(specialKeys.descendants) do
		key:apply(value, applyTo, cleanupTasks)
	end

	if props.Parent ~= nil then
		bindProperty(applyTo, "Parent", props.Parent, cleanupTasks)
	end

	for key, value in pairs(specialKeys.ancestor) do
		key:apply(value, applyTo, cleanupTasks)
	end
	for key, value in pairs(specialKeys.observer) do
		key:apply(value, applyTo, cleanupTasks)
	end

	applyTo.Destroying:Connect(function()
		cleanup(cleanupTasks)
	end)
end

return applyInstanceProps
end)() end,
    [32] = function()local wax,script,require=ImportGlobals(32)local ImportGlobals return (function(...)--!strict

--[[
	Stores 'sensible default' properties to be applied to instances created by
	the New function.
]]

return {
	ScreenGui = {
		ResetOnSpawn = false,
		ZIndexBehavior = Enum.ZIndexBehavior.Sibling
	},

	BillboardGui = {
		ResetOnSpawn = false,
		ZIndexBehavior = Enum.ZIndexBehavior.Sibling
	},

	SurfaceGui = {
		ResetOnSpawn = false,
		ZIndexBehavior = Enum.ZIndexBehavior.Sibling,

		SizingMode = Enum.SurfaceGuiSizingMode.PixelsPerStud,
		PixelsPerStud = 50
	},

	Frame = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0
	},

	ScrollingFrame = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0,

		ScrollBarImageColor3 = Color3.new(0, 0, 0)
	},

	TextLabel = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0,

		Font = Enum.Font.SourceSans,
		Text = "",
		TextColor3 = Color3.new(0, 0, 0),
		TextSize = 14
	},

	TextButton = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0,

		AutoButtonColor = false,

		Font = Enum.Font.SourceSans,
		Text = "",
		TextColor3 = Color3.new(0, 0, 0),
		TextSize = 14
	},

	TextBox = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0,

		ClearTextOnFocus = false,

		Font = Enum.Font.SourceSans,
		Text = "",
		TextColor3 = Color3.new(0, 0, 0),
		TextSize = 14
	},

	ImageLabel = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0
	},

	ImageButton = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0,

		AutoButtonColor = false
	},

	ViewportFrame = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0
	},

	VideoFrame = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0
	},
	
	CanvasGroup = {
		BackgroundColor3 = Color3.new(1, 1, 1),
		BorderColor3 = Color3.new(0, 0, 0),
		BorderSizePixel = 0
	}
}

end)() end,
    [34] = function()local wax,script,require=ImportGlobals(34)local ImportGlobals return (function(...)--!strict

--[[
	Utility function to log a Fusion-specific error.
]]

local Package = script.Parent.Parent
local Types = require(Package.Types)
local messages = require(Package.Logging.messages)

local function logError(messageID: string, errObj: Types.Error?, ...)
	local formatString: string

	if messages[messageID] ~= nil then
		formatString = messages[messageID]
	else
		messageID = "unknownMessage"
		formatString = messages[messageID]
	end

	local errorString
	if errObj == nil then
		errorString = string.format("[Fusion] " .. formatString .. "\n(ID: " .. messageID .. ")", ...)
	else
		formatString = formatString:gsub("ERROR_MESSAGE", errObj.message)
		errorString = string.format("[Fusion] " .. formatString .. "\n(ID: " .. messageID .. ")\n---- Stack trace ----\n" .. errObj.trace, ...)
	end

	error(errorString:gsub("\n", "\n    "), 0)
end

return logError
end)() end,
    [35] = function()local wax,script,require=ImportGlobals(35)local ImportGlobals return (function(...)--!strict

--[[
	Utility function to log a Fusion-specific error, without halting execution.
]]

local Package = script.Parent.Parent
local Types = require(Package.Types)
local messages = require(Package.Logging.messages)

local function logErrorNonFatal(messageID: string, errObj: Types.Error?, ...)
	local formatString: string

	if messages[messageID] ~= nil then
		formatString = messages[messageID]
	else
		messageID = "unknownMessage"
		formatString = messages[messageID]
	end

	local errorString
	if errObj == nil then
		errorString = string.format("[Fusion] " .. formatString .. "\n(ID: " .. messageID .. ")", ...)
	else
		formatString = formatString:gsub("ERROR_MESSAGE", errObj.message)
		errorString = string.format("[Fusion] " .. formatString .. "\n(ID: " .. messageID .. ")\n---- Stack trace ----\n" .. errObj.trace, ...)
	end

	task.spawn(function(...)
		error(errorString:gsub("\n", "\n    "), 0)
	end, ...)
end

return logErrorNonFatal
end)() end,
    [36] = function()local wax,script,require=ImportGlobals(36)local ImportGlobals return (function(...)--!strict

--[[
	Utility function to log a Fusion-specific warning.
]]

local Package = script.Parent.Parent
local messages = require(Package.Logging.messages)

local function logWarn(messageID, ...)
	local formatString: string

	if messages[messageID] ~= nil then
		formatString = messages[messageID]
	else
		messageID = "unknownMessage"
		formatString = messages[messageID]
	end

	warn(string.format("[Fusion] " .. formatString .. "\n(ID: " .. messageID .. ")", ...))
end

return logWarn
end)() end,
    [37] = function()local wax,script,require=ImportGlobals(37)local ImportGlobals return (function(...)--!strict

--[[
	Stores templates for different kinds of logging messages.
]]

return {
	cannotAssignProperty = "The class type '%s' has no assignable property '%s'.",
	cannotConnectChange = "The %s class doesn't have a property called '%s'.",
	cannotConnectEvent = "The %s class doesn't have an event called '%s'.",
	cannotCreateClass = "Can't create a new instance of class '%s'.",
	computedCallbackError = "Computed callback error: ERROR_MESSAGE",
	destructorNeededValue = "To save instances into Values, provide a destructor function. This will be an error soon - see discussion #183 on GitHub.",
	destructorNeededComputed = "To return instances from Computeds, provide a destructor function. This will be an error soon - see discussion #183 on GitHub.",
	multiReturnComputed = "Returning multiple values from Computeds is discouraged, as behaviour will change soon - see discussion #189 on GitHub.",
	destructorNeededForKeys = "To return instances from ForKeys, provide a destructor function. This will be an error soon - see discussion #183 on GitHub.",
	destructorNeededForValues = "To return instances from ForValues, provide a destructor function. This will be an error soon - see discussion #183 on GitHub.",
	destructorNeededForPairs = "To return instances from ForPairs, provide a destructor function. This will be an error soon - see discussion #183 on GitHub.",
	duplicatePropertyKey = "",
	forKeysProcessorError = "ForKeys callback error: ERROR_MESSAGE",
	forKeysKeyCollision = "ForKeys should only write to output key '%s' once when processing key changes, but it wrote to it twice. Previously input key: '%s'; New input key: '%s'",
	forKeysDestructorError = "ForKeys destructor error: ERROR_MESSAGE",
	forPairsDestructorError = "ForPairs destructor error: ERROR_MESSAGE",
	forPairsKeyCollision = "ForPairs should only write to output key '%s' once when processing key changes, but it wrote to it twice. Previous input pair: '[%s] = %s'; New input pair: '[%s] = %s'",
	forPairsProcessorError = "ForPairs callback error: ERROR_MESSAGE",
	forValuesProcessorError = "ForValues callback error: ERROR_MESSAGE",
	forValuesDestructorError = "ForValues destructor error: ERROR_MESSAGE",
	invalidChangeHandler = "The change handler for the '%s' property must be a function.",
	invalidEventHandler = "The handler for the '%s' event must be a function.",
	invalidPropertyType = "'%s.%s' expected a '%s' type, but got a '%s' type.",
	invalidRefType = "Instance refs must be Value objects.",
	invalidOutType = "[Out] properties must be given Value objects.",
	invalidOutProperty = "The %s class doesn't have a property called '%s'.",
	invalidSpringDamping = "The damping ratio for a spring must be >= 0. (damping was %.2f)",
	invalidSpringSpeed = "The speed of a spring must be >= 0. (speed was %.2f)",
	mistypedSpringDamping = "The damping ratio for a spring must be a number. (got a %s)",
	mistypedSpringSpeed = "The speed of a spring must be a number. (got a %s)",
	mistypedTweenInfo = "The tween info of a tween must be a TweenInfo. (got a %s)",
	springTypeMismatch = "The type '%s' doesn't match the spring's type '%s'.",
	strictReadError = "'%s' is not a valid member of '%s'.",
	unknownMessage = "Unknown error: ERROR_MESSAGE",
	unrecognisedChildType = "'%s' type children aren't accepted by `[Children]`.",
	unrecognisedPropertyKey = "'%s' keys aren't accepted in property tables.",
	unrecognisedPropertyStage = "'%s' isn't a valid stage for a special key to be applied at."
}
end)() end,
    [38] = function()local wax,script,require=ImportGlobals(38)local ImportGlobals return (function(...)--!strict

--[[
	An xpcall() error handler to collect and parse useful information about
	errors, such as clean messages and stack traces.

	TODO: this should have a 'type' field for runtime type checking!
]]

local Package = script.Parent.Parent
local Types = require(Package.Types)

local function parseError(err: string): Types.Error
	return {
		type = "Error",
		raw = err,
		message = err:gsub("^.+:%d+:%s*", ""),
		trace = debug.traceback(nil, 2)
	}
end

return parseError
end)() end,
    [39] = function()local wax,script,require=ImportGlobals(39)local ImportGlobals return (function(...)--!strict

--[[
	Stores common public-facing type information for Fusion APIs.
]]

type Set<T> = {[T]: any}

--[[
	General use types
]]

-- A unique symbolic value.
export type Symbol = {
	type: string, -- replace with "Symbol" when Luau supports singleton types
	name: string
}

-- Types that can be expressed as vectors of numbers, and so can be animated.
export type Animatable =
	number |
	CFrame |
	Color3 |
	ColorSequenceKeypoint |
	DateTime |
	NumberRange |
	NumberSequenceKeypoint |
	PhysicalProperties |
	Ray |
	Rect |
	Region3 |
	Region3int16 |
	UDim |
	UDim2 |
	Vector2 |
	Vector2int16 |
	Vector3 |
	Vector3int16

-- A task which can be accepted for cleanup.
export type Task =
	Instance |
	RBXScriptConnection |
	() -> () |
	{destroy: (any) -> ()} |
	{Destroy: (any) -> ()} |
	{Task}

-- Script-readable version information.
export type Version = {
	major: number,
	minor: number,
	isRelease: boolean
}
--[[
	Generic reactive graph types
]]

-- A graph object which can have dependents.
export type Dependency = {
	dependentSet: Set<Dependent>
}

-- A graph object which can have dependencies.
export type Dependent = {
	update: (Dependent) -> boolean,
	dependencySet: Set<Dependency>
}

-- An object which stores a piece of reactive state.
export type StateObject<T> = Dependency & {
	type: string, -- replace with "State" when Luau supports singleton types
	kind: string,
	get: (StateObject<T>, asDependency: boolean?) -> T
}

-- Either a constant value of type T, or a state object containing type T.
export type CanBeState<T> = StateObject<T> | T

--[[
	Specific reactive graph types
]]

-- A state object whose value can be set at any time by the user.
export type Value<T> = StateObject<T> & {
	-- kind: "State" (add this when Luau supports singleton types)
 	set: (Value<T>, newValue: any, force: boolean?) -> ()
}

-- A state object whose value is derived from other objects using a callback.
export type Computed<T> = StateObject<T> & Dependent & {
	-- kind: "Computed" (add this when Luau supports singleton types)
}

-- A state object whose value is derived from other objects using a callback.
export type ForPairs<KO, VO> = StateObject<{ [KO]: VO }> & Dependent & {
	-- kind: "ForPairs" (add this when Luau supports singleton types)
}
-- A state object whose value is derived from other objects using a callback.
export type ForKeys<KO, V> = StateObject<{ [KO]: V }> & Dependent & {
	-- kind: "ForKeys" (add this when Luau supports singleton types)
}
-- A state object whose value is derived from other objects using a callback.
export type ForValues<K, VO> = StateObject<{ [K]: VO }> & Dependent & {
	-- kind: "ForKeys" (add this when Luau supports singleton types)
}

-- A state object which follows another state object using tweens.
export type Tween<T> = StateObject<T> & Dependent & {
	-- kind: "Tween" (add this when Luau supports singleton types)
}

-- A state object which follows another state object using spring simulation.
export type Spring<T> = StateObject<T> & Dependent & {
	-- kind: "Spring" (add this when Luau supports singleton types)
	-- Uncomment when ENABLE_PARAM_SETTERS is enabled
	-- setPosition: (Spring<T>, newValue: Animatable) -> (),
	-- setVelocity: (Spring<T>, newValue: Animatable) -> (),
	-- addVelocity: (Spring<T>, deltaValue: Animatable) -> ()
}

-- An object which can listen for updates on another state object.
export type Observer = Dependent & {
	-- kind: "Observer" (add this when Luau supports singleton types)
  	onChange: (Observer, callback: () -> ()) -> (() -> ())
}

--[[
	Instance related types
]]

-- Denotes children instances in an instance or component's property table.
export type SpecialKey = {
	type: string, -- replace with "SpecialKey" when Luau supports singleton types
	kind: string,
	stage: string, -- replace with "self" | "descendants" | "ancestor" | "observer" when Luau supports singleton types
	apply: (SpecialKey, value: any, applyTo: Instance, cleanupTasks: {Task}) -> ()
}

-- A collection of instances that may be parented to another instance.
export type Children = Instance | StateObject<Children> | {[any]: Children}

-- A table that defines an instance's properties, handlers and children.
export type PropertyTable = {[string | SpecialKey]: any}

return nil
end)() end,
    [41] = function()local wax,script,require=ImportGlobals(41)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs and returns objects which can be used to model derived reactive
	state.
]]

local Package = script.Parent.Parent
local Types = require(Package.Types)
local captureDependencies = require(Package.Dependencies.captureDependencies)
local initDependency = require(Package.Dependencies.initDependency)
local useDependency = require(Package.Dependencies.useDependency)
local logErrorNonFatal = require(Package.Logging.logErrorNonFatal)
local logWarn = require(Package.Logging.logWarn)
local isSimilar = require(Package.Utility.isSimilar)
local needsDestruction = require(Package.Utility.needsDestruction)

local class = {}

local CLASS_METATABLE = {__index = class}
local WEAK_KEYS_METATABLE = {__mode = "k"}

--[[
	Returns the last cached value calculated by this Computed object.
	The computed object will be registered as a dependency unless `asDependency`
	is false.
]]
function class:get(asDependency: boolean?): any
	if asDependency ~= false then
		useDependency(self)
	end
	return self._value
end

--[[
	Recalculates this Computed's cached value and dependencies.
	Returns true if it changed, or false if it's identical.
]]
function class:update(): boolean
	-- remove this object from its dependencies' dependent sets
	for dependency in pairs(self.dependencySet) do
		dependency.dependentSet[self] = nil
	end

	-- we need to create a new, empty dependency set to capture dependencies
	-- into, but in case there's an error, we want to restore our old set of
	-- dependencies. by using this table-swapping solution, we can avoid the
	-- overhead of allocating new tables each update.
	self._oldDependencySet, self.dependencySet = self.dependencySet, self._oldDependencySet
	table.clear(self.dependencySet)

	local ok, newValue, newMetaValue = captureDependencies(self.dependencySet, self._processor)

	if ok then
		if self._destructor == nil and needsDestruction(newValue) then
			logWarn("destructorNeededComputed")
		end

		if newMetaValue ~= nil then
			logWarn("multiReturnComputed")
		end

		local oldValue = self._value
		local similar = isSimilar(oldValue, newValue)
		if self._destructor ~= nil then
			self._destructor(oldValue)
		end
		self._value = newValue

		-- add this object to the dependencies' dependent sets
		for dependency in pairs(self.dependencySet) do
			dependency.dependentSet[self] = true
		end

		return not similar
	else
		-- this needs to be non-fatal, because otherwise it'd disrupt the
		-- update process
		logErrorNonFatal("computedCallbackError", newValue)

		-- restore old dependencies, because the new dependencies may be corrupt
		self._oldDependencySet, self.dependencySet = self.dependencySet, self._oldDependencySet

		-- restore this object in the dependencies' dependent sets
		for dependency in pairs(self.dependencySet) do
			dependency.dependentSet[self] = true
		end

		return false
	end
end

local function Computed<T>(processor: () -> T, destructor: ((T) -> ())?): Types.Computed<T>
	local self = setmetatable({
		type = "State",
		kind = "Computed",
		dependencySet = {},
		-- if we held strong references to the dependents, then they wouldn't be
		-- able to get garbage collected when they fall out of scope
		dependentSet = setmetatable({}, WEAK_KEYS_METATABLE),
		_oldDependencySet = {},
		_processor = processor,
		_destructor = destructor,
		_value = nil,
	}, CLASS_METATABLE)

	initDependency(self)
	self:update()

	return self
end

return Computed
end)() end,
    [42] = function()local wax,script,require=ImportGlobals(42)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs a new ForKeys state object which maps keys of an array using
	a `processor` function.

	Optionally, a `destructor` function can be specified for cleaning up
	calculated keys. If omitted, the default cleanup function will be used instead.

	Optionally, a `meta` value can be returned in the processor function as the
	second value to pass data from the processor to the destructor.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Types = require(Package.Types)
local captureDependencies = require(Package.Dependencies.captureDependencies)
local initDependency = require(Package.Dependencies.initDependency)
local useDependency = require(Package.Dependencies.useDependency)
local parseError = require(Package.Logging.parseError)
local logErrorNonFatal = require(Package.Logging.logErrorNonFatal)
local logError = require(Package.Logging.logError)
local logWarn = require(Package.Logging.logWarn)
local cleanup = require(Package.Utility.cleanup)
local needsDestruction = require(Package.Utility.needsDestruction)

local class = {}

local CLASS_METATABLE = { __index = class }
local WEAK_KEYS_METATABLE = { __mode = "k" }

--[[
	Returns the current value of this ForKeys object.
	The object will be registered as a dependency unless `asDependency` is false.
]]
function class:get(asDependency: boolean?): any
	if asDependency ~= false then
		useDependency(self)
	end
	return self._outputTable
end


--[[
	Called when the original table is changed.

	This will firstly find any keys meeting any of the following criteria:

	- they were not previously present
	- a dependency used during generation of this value has changed

	It will recalculate those key pairs, storing information about any
	dependencies used in the processor callback during output key generation,
	and save the new key to the output array with the same value. If it is
	overwriting an older value, that older value will be passed to the
	destructor for cleanup.

	Finally, this function will find keys that are no longer present, and remove
	their output keys from the output table and pass them to the destructor.
]]

function class:update(): boolean
	local inputIsState = self._inputIsState
	local newInputTable = if inputIsState then self._inputTable:get(false) else self._inputTable
	local oldInputTable = self._oldInputTable
	local outputTable = self._outputTable

	local keyOIMap = self._keyOIMap
	local keyIOMap = self._keyIOMap
	local meta = self._meta

	local didChange = false


	-- clean out main dependency set
	for dependency in pairs(self.dependencySet) do
		dependency.dependentSet[self] = nil
	end

	self._oldDependencySet, self.dependencySet = self.dependencySet, self._oldDependencySet
	table.clear(self.dependencySet)

	-- if the input table is a state object, add it as a dependency
	if inputIsState then
		self._inputTable.dependentSet[self] = true
		self.dependencySet[self._inputTable] = true
	end


	-- STEP 1: find keys that changed or were not previously present
	for newInKey, value in pairs(newInputTable) do
		-- get or create key data
		local keyData = self._keyData[newInKey]

		if keyData == nil then
			keyData = {
				dependencySet = setmetatable({}, WEAK_KEYS_METATABLE),
				oldDependencySet = setmetatable({}, WEAK_KEYS_METATABLE),
				dependencyValues = setmetatable({}, WEAK_KEYS_METATABLE),
			}
			self._keyData[newInKey] = keyData
		end

		-- check if the key is new
		local shouldRecalculate = oldInputTable[newInKey] == nil

		-- check if the key's dependencies have changed
		if shouldRecalculate == false then
			for dependency, oldValue in pairs(keyData.dependencyValues) do
				if oldValue ~= dependency:get(false) then
					shouldRecalculate = true
					break
				end
			end
		end


		-- recalculate the output key if necessary
		if shouldRecalculate then
			keyData.oldDependencySet, keyData.dependencySet = keyData.dependencySet, keyData.oldDependencySet
			table.clear(keyData.dependencySet)

			local processOK, newOutKey, newMetaValue = captureDependencies(
				keyData.dependencySet,
				self._processor,
				newInKey
			)

			if processOK then
				if self._destructor == nil and (needsDestruction(newOutKey) or needsDestruction(newMetaValue)) then
					logWarn("destructorNeededForKeys")
				end

				local oldInKey = keyOIMap[newOutKey]
				local oldOutKey = keyIOMap[newInKey]

				-- check for key collision
				if oldInKey ~= newInKey and newInputTable[oldInKey] ~= nil then
					logError("forKeysKeyCollision", nil, tostring(newOutKey), tostring(oldInKey), tostring(newOutKey))
				end

				-- check for a changed output key
				if oldOutKey ~= newOutKey and keyOIMap[oldOutKey] == newInKey then
					-- clean up the old calculated value
					local oldMetaValue = meta[oldOutKey]

					local destructOK, err = xpcall(self._destructor or cleanup, parseError, oldOutKey, oldMetaValue)
					if not destructOK then
						logErrorNonFatal("forKeysDestructorError", err)
					end

					keyOIMap[oldOutKey] = nil
					outputTable[oldOutKey] = nil
					meta[oldOutKey] = nil
				end

				-- update the stored data for this key
				oldInputTable[newInKey] = value
				meta[newOutKey] = newMetaValue
				keyOIMap[newOutKey] = newInKey
				keyIOMap[newInKey] = newOutKey
				outputTable[newOutKey] = value

				-- if we had to recalculate the output, then we did change
				didChange = true
			else
				-- restore old dependencies, because the new dependencies may be corrupt
				keyData.oldDependencySet, keyData.dependencySet = keyData.dependencySet, keyData.oldDependencySet

				logErrorNonFatal("forKeysProcessorError", newOutKey)
			end
		end


		-- save dependency values and add to main dependency set
		for dependency in pairs(keyData.dependencySet) do
			keyData.dependencyValues[dependency] = dependency:get(false)

			self.dependencySet[dependency] = true
			dependency.dependentSet[self] = true
		end
	end


	-- STEP 2: find keys that were removed
	for outputKey, inputKey in pairs(keyOIMap) do
		if newInputTable[inputKey] == nil then
			-- clean up the old calculated value
			local oldMetaValue = meta[outputKey]

			local destructOK, err = xpcall(self._destructor or cleanup, parseError, outputKey, oldMetaValue)
			if not destructOK then
				logErrorNonFatal("forKeysDestructorError", err)
			end

			-- remove data
			oldInputTable[inputKey] = nil
			meta[outputKey] = nil
			keyOIMap[outputKey] = nil
			keyIOMap[inputKey] = nil
			outputTable[outputKey] = nil
			self._keyData[inputKey] = nil

			-- if we removed a key, then the table/state changed
			didChange = true
		end
	end

	return didChange
end

local function ForKeys<KI, KO, M>(
	inputTable: PubTypes.CanBeState<{ [KI]: any }>,
	processor: (KI) -> (KO, M?),
	destructor: (KO, M?) -> ()?
): Types.ForKeys<KI, KO, M>

	local inputIsState = inputTable.type == "State" and typeof(inputTable.get) == "function"

	local self = setmetatable({
		type = "State",
		kind = "ForKeys",
		dependencySet = {},
		-- if we held strong references to the dependents, then they wouldn't be
		-- able to get garbage collected when they fall out of scope
		dependentSet = setmetatable({}, WEAK_KEYS_METATABLE),
		_oldDependencySet = {},

		_processor = processor,
		_destructor = destructor,
		_inputIsState = inputIsState,

		_inputTable = inputTable,
		_oldInputTable = {},
		_outputTable = {},
		_keyOIMap = {},
		_keyIOMap = {},
		_keyData = {},
		_meta = {},
	}, CLASS_METATABLE)

	initDependency(self)
	self:update()

	return self
end

return ForKeys
end)() end,
    [43] = function()local wax,script,require=ImportGlobals(43)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs a new ForPairs object which maps pairs of a table using
	a `processor` function.

	Optionally, a `destructor` function can be specified for cleaning up values.
	If omitted, the default cleanup function will be used instead.

	Additionally, a `meta` table/value can optionally be returned to pass data created
	when running the processor to the destructor when the created object is cleaned up.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Types = require(Package.Types)
local captureDependencies = require(Package.Dependencies.captureDependencies)
local initDependency = require(Package.Dependencies.initDependency)
local useDependency = require(Package.Dependencies.useDependency)
local parseError = require(Package.Logging.parseError)
local logErrorNonFatal = require(Package.Logging.logErrorNonFatal)
local logError = require(Package.Logging.logError)
local logWarn = require(Package.Logging.logWarn)
local cleanup = require(Package.Utility.cleanup)
local needsDestruction = require(Package.Utility.needsDestruction)

local class = {}

local CLASS_METATABLE = { __index = class }
local WEAK_KEYS_METATABLE = { __mode = "k" }

--[[
	Returns the current value of this ForPairs object.
	The object will be registered as a dependency unless `asDependency` is false.
]]
function class:get(asDependency: boolean?): any
	if asDependency ~= false then
		useDependency(self)
	end
	return self._outputTable
end

--[[
	Called when the original table is changed.

	This will firstly find any keys meeting any of the following criteria:

	- they were not previously present
	- their associated value has changed
	- a dependency used during generation of this value has changed

	It will recalculate those key/value pairs, storing information about any
	dependencies used in the processor callback during value generation, and
	save the new key/value pair to the output array. If it is overwriting an
	older key/value pair, that older pair will be passed to the destructor
	for cleanup.

	Finally, this function will find keys that are no longer present, and remove
	their key/value pairs from the output table and pass them to the destructor.
]]
function class:update(): boolean
	local inputIsState = self._inputIsState
	local newInputTable = if inputIsState then self._inputTable:get(false) else self._inputTable
	local oldInputTable = self._oldInputTable

	local keyIOMap = self._keyIOMap
	local meta = self._meta

	local didChange = false


	-- clean out main dependency set
	for dependency in pairs(self.dependencySet) do
		dependency.dependentSet[self] = nil
	end

	self._oldDependencySet, self.dependencySet = self.dependencySet, self._oldDependencySet
	table.clear(self.dependencySet)

	-- if the input table is a state object, add it as a dependency
	if inputIsState then
		self._inputTable.dependentSet[self] = true
		self.dependencySet[self._inputTable] = true
	end

	-- clean out output table
	self._oldOutputTable, self._outputTable = self._outputTable, self._oldOutputTable

	local oldOutputTable = self._oldOutputTable
	local newOutputTable = self._outputTable
	table.clear(newOutputTable)

	-- Step 1: find key/value pairs that changed or were not previously present

	for newInKey, newInValue in pairs(newInputTable) do
		-- get or create key data
		local keyData = self._keyData[newInKey]

		if keyData == nil then
			keyData = {
				dependencySet = setmetatable({}, WEAK_KEYS_METATABLE),
				oldDependencySet = setmetatable({}, WEAK_KEYS_METATABLE),
				dependencyValues = setmetatable({}, WEAK_KEYS_METATABLE),
			}
			self._keyData[newInKey] = keyData
		end


		-- check if the pair is new or changed
		local shouldRecalculate = oldInputTable[newInKey] ~= newInValue

		-- check if the pair's dependencies have changed
		if shouldRecalculate == false then
			for dependency, oldValue in pairs(keyData.dependencyValues) do
				if oldValue ~= dependency:get(false) then
					shouldRecalculate = true
					break
				end
			end
		end


		-- recalculate the output pair if necessary
		if shouldRecalculate then
			keyData.oldDependencySet, keyData.dependencySet = keyData.dependencySet, keyData.oldDependencySet
			table.clear(keyData.dependencySet)

			local processOK, newOutKey, newOutValue, newMetaValue = captureDependencies(
				keyData.dependencySet,
				self._processor,
				newInKey,
				newInValue
			)

			if processOK then
				if self._destructor == nil and (needsDestruction(newOutKey) or needsDestruction(newOutValue) or needsDestruction(newMetaValue)) then
					logWarn("destructorNeededForPairs")
				end

				-- if this key was already written to on this run-through, throw a fatal error.
				if newOutputTable[newOutKey] ~= nil then
					-- figure out which key/value pair previously wrote to this key
					local previousNewKey, previousNewValue
					for inKey, outKey in pairs(keyIOMap) do
						if outKey == newOutKey then
							previousNewValue = newInputTable[inKey]
							if previousNewValue ~= nil then
								previousNewKey = inKey
								break
							end
						end
					end

					if previousNewKey ~= nil then
						logError(
							"forPairsKeyCollision",
							nil,
							tostring(newOutKey),
							tostring(previousNewKey),
							tostring(previousNewValue),
							tostring(newInKey),
							tostring(newInValue)
						)
					end
				end

				local oldOutValue = oldOutputTable[newOutKey]

				if oldOutValue ~= newOutValue then
					local oldMetaValue = meta[newOutKey]
					if oldOutValue ~= nil then
						local destructOK, err = xpcall(self._destructor or cleanup, parseError, newOutKey, oldOutValue, oldMetaValue)
						if not destructOK then
							logErrorNonFatal("forPairsDestructorError", err)
						end
					end

					oldOutputTable[newOutKey] = nil
				end

				-- update the stored data for this key/value pair
				oldInputTable[newInKey] = newInValue
				keyIOMap[newInKey] = newOutKey
				meta[newOutKey] = newMetaValue
				newOutputTable[newOutKey] = newOutValue

				-- if we had to recalculate the output, then we did change
				didChange = true
			else
				-- restore old dependencies, because the new dependencies may be corrupt
				keyData.oldDependencySet, keyData.dependencySet = keyData.dependencySet, keyData.oldDependencySet

				logErrorNonFatal("forPairsProcessorError", newOutKey)
			end
		else
			local storedOutKey = keyIOMap[newInKey]

			-- check for key collision
			if newOutputTable[storedOutKey] ~= nil then
				-- figure out which key/value pair previously wrote to this key
				local previousNewKey, previousNewValue
				for inKey, outKey in pairs(keyIOMap) do
					if storedOutKey == outKey then
						previousNewValue = newInputTable[inKey]

						if previousNewValue ~= nil then
							previousNewKey = inKey
							break
						end
					end
				end

				if previousNewKey ~= nil then
					logError(
						"forPairsKeyCollision",
						nil,
						tostring(storedOutKey),
						tostring(previousNewKey),
						tostring(previousNewValue),
						tostring(newInKey),
						tostring(newInValue)
					)
				end
			end

			-- copy the stored key/value pair into the new output table
			newOutputTable[storedOutKey] = oldOutputTable[storedOutKey]
		end


		-- save dependency values and add to main dependency set
		for dependency in pairs(keyData.dependencySet) do
			keyData.dependencyValues[dependency] = dependency:get(false)

			self.dependencySet[dependency] = true
			dependency.dependentSet[self] = true
		end
	end

	-- STEP 2: find keys that were removed
	for oldOutKey, oldOutValue in pairs(oldOutputTable) do
		-- check if this key/value pair is in the new output table
		if newOutputTable[oldOutKey] ~= oldOutValue then
			-- clean up the old output pair
			local oldMetaValue = meta[oldOutKey]
			if oldOutValue ~= nil then
				local destructOK, err = xpcall(self._destructor or cleanup, parseError, oldOutKey, oldOutValue, oldMetaValue)
				if not destructOK then
					logErrorNonFatal("forPairsDestructorError", err)
				end
			end

			-- check if the key was completely removed from the output table
			if newOutputTable[oldOutKey] == nil then
				meta[oldOutKey] = nil
				self._keyData[oldOutKey] = nil
			end

			didChange = true
		end
	end

	for key in pairs(oldInputTable) do
		if newInputTable[key] == nil then
			oldInputTable[key] = nil
			keyIOMap[key] = nil
		end
	end

	return didChange
end

local function ForPairs<KI, VI, KO, VO, M>(
	inputTable: PubTypes.CanBeState<{ [KI]: VI }>,
	processor: (KI, VI) -> (KO, VO, M?),
	destructor: (KO, VO, M?) -> ()?
): Types.ForPairs<KI, VI, KO, VO, M>

	local inputIsState = inputTable.type == "State" and typeof(inputTable.get) == "function"

	local self = setmetatable({
		type = "State",
		kind = "ForPairs",
		dependencySet = {},
		-- if we held strong references to the dependents, then they wouldn't be
		-- able to get garbage collected when they fall out of scope
		dependentSet = setmetatable({}, WEAK_KEYS_METATABLE),
		_oldDependencySet = {},

		_processor = processor,
		_destructor = destructor,
		_inputIsState = inputIsState,

		_inputTable = inputTable,
		_oldInputTable = {},
		_outputTable = {},
		_oldOutputTable = {},
		_keyIOMap = {},
		_keyData = {},
		_meta = {},
	}, CLASS_METATABLE)

	initDependency(self)
	self:update()

	return self
end

return ForPairs
end)() end,
    [44] = function()local wax,script,require=ImportGlobals(44)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs a new ForValues object which maps values of a table using
	a `processor` function.

	Optionally, a `destructor` function can be specified for cleaning up values.
	If omitted, the default cleanup function will be used instead.

	Additionally, a `meta` table/value can optionally be returned to pass data created
	when running the processor to the destructor when the created object is cleaned up.
]]
local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Types = require(Package.Types)
local captureDependencies = require(Package.Dependencies.captureDependencies)
local initDependency = require(Package.Dependencies.initDependency)
local useDependency = require(Package.Dependencies.useDependency)
local parseError = require(Package.Logging.parseError)
local logErrorNonFatal = require(Package.Logging.logErrorNonFatal)
local logWarn = require(Package.Logging.logWarn)
local cleanup = require(Package.Utility.cleanup)
local needsDestruction = require(Package.Utility.needsDestruction)

local class = {}

local CLASS_METATABLE = { __index = class }
local WEAK_KEYS_METATABLE = { __mode = "k" }

--[[
	Returns the current value of this ForValues object.
	The object will be registered as a dependency unless `asDependency` is false.
]]
function class:get(asDependency: boolean?): any
	if asDependency ~= false then
		useDependency(self)
	end
	return self._outputTable
end

--[[
	Called when the original table is changed.

	This will firstly find any values meeting any of the following criteria:

	- they were not previously present
	- a dependency used during generation of this value has changed

	It will recalculate those values, storing information about any dependencies
	used in the processor callback during value generation, and save the new value
	to the output array with the same key. If it is overwriting an older value,
	that older value will be passed to the destructor for cleanup.

	Finally, this function will find values that are no longer present, and remove
	their values from the output table and pass them to the destructor. You can re-use
	the same value multiple times and this will function will update them as little as
	possible; reusing the same values where possible.
]]
function class:update(): boolean
	local inputIsState = self._inputIsState
	local inputTable = if inputIsState then self._inputTable:get(false) else self._inputTable
	local outputValues = {}

	local didChange = false

	-- clean out value cache
	self._oldValueCache, self._valueCache = self._valueCache, self._oldValueCache
	local newValueCache = self._valueCache
	local oldValueCache = self._oldValueCache
	table.clear(newValueCache)

	-- clean out main dependency set
	for dependency in pairs(self.dependencySet) do
		dependency.dependentSet[self] = nil
	end
	self._oldDependencySet, self.dependencySet = self.dependencySet, self._oldDependencySet
	table.clear(self.dependencySet)

	-- if the input table is a state object, add it as a dependency
	if inputIsState then
		self._inputTable.dependentSet[self] = true
		self.dependencySet[self._inputTable] = true
	end


	-- STEP 1: find values that changed or were not previously present
	for inKey, inValue in pairs(inputTable) do
		-- check if the value is new or changed
		local oldCachedValues = oldValueCache[inValue]
		local shouldRecalculate = oldCachedValues == nil

		-- get a cached value and its dependency/meta data if available
		local value, valueData, meta

		if type(oldCachedValues) == "table" and #oldCachedValues > 0 then
			local valueInfo = table.remove(oldCachedValues, #oldCachedValues)
			value = valueInfo.value
			valueData = valueInfo.valueData
			meta = valueInfo.meta

			if #oldCachedValues <= 0 then
				oldValueCache[inValue] = nil
			end
		elseif oldCachedValues ~= nil then
			oldValueCache[inValue] = nil
			shouldRecalculate = true
		end

		if valueData == nil then
			valueData = {
				dependencySet = setmetatable({}, WEAK_KEYS_METATABLE),
				oldDependencySet = setmetatable({}, WEAK_KEYS_METATABLE),
				dependencyValues = setmetatable({}, WEAK_KEYS_METATABLE),
			}
		end

		-- check if the value's dependencies have changed
		if shouldRecalculate == false then
			for dependency, oldValue in pairs(valueData.dependencyValues) do
				if oldValue ~= dependency:get(false) then
					shouldRecalculate = true
					break
				end
			end
		end

		-- recalculate the output value if necessary
		if shouldRecalculate then
			valueData.oldDependencySet, valueData.dependencySet = valueData.dependencySet, valueData.oldDependencySet
			table.clear(valueData.dependencySet)

			local processOK, newOutValue, newMetaValue = captureDependencies(
				valueData.dependencySet,
				self._processor,
				inValue
			)

			if processOK then
				if self._destructor == nil and (needsDestruction(newOutValue) or needsDestruction(newMetaValue)) then
					logWarn("destructorNeededForValues")
				end

				-- pass the old value to the destructor if it exists
				if value ~= nil then
					local destructOK, err = xpcall(self._destructor or cleanup, parseError, value, meta)
					if not destructOK then
						logErrorNonFatal("forValuesDestructorError", err)
					end
				end

				-- store the new value and meta data
				value = newOutValue
				meta = newMetaValue
				didChange = true
			else
				-- restore old dependencies, because the new dependencies may be corrupt
				valueData.oldDependencySet, valueData.dependencySet = valueData.dependencySet, valueData.oldDependencySet

				logErrorNonFatal("forValuesProcessorError", newOutValue)
			end
		end


		-- store the value and its dependency/meta data
		local newCachedValues = newValueCache[inValue]
		if newCachedValues == nil then
			newCachedValues = {}
			newValueCache[inValue] = newCachedValues
		end

		table.insert(newCachedValues, {
			value = value,
			valueData = valueData,
			meta = meta,
		})

		outputValues[inKey] = value


		-- save dependency values and add to main dependency set
		for dependency in pairs(valueData.dependencySet) do
			valueData.dependencyValues[dependency] = dependency:get(false)

			self.dependencySet[dependency] = true
			dependency.dependentSet[self] = true
		end
	end


	-- STEP 2: find values that were removed
	-- for tables of data, we just need to check if it's still in the cache
	for _oldInValue, oldCachedValueInfo in pairs(oldValueCache) do
		for _, valueInfo in ipairs(oldCachedValueInfo) do
			local oldValue = valueInfo.value
			local oldMetaValue = valueInfo.meta

			local destructOK, err = xpcall(self._destructor or cleanup, parseError, oldValue, oldMetaValue)
			if not destructOK then
				logErrorNonFatal("forValuesDestructorError", err)
			end

			didChange = true
		end

		table.clear(oldCachedValueInfo)
	end

	self._outputTable = outputValues

	return didChange
end

local function ForValues<VI, VO, M>(
	inputTable: PubTypes.CanBeState<{ [any]: VI }>,
	processor: (VI) -> (VO, M?),
	destructor: (VO, M?) -> ()?
): Types.ForValues<VI, VO, M>

	local inputIsState = inputTable.type == "State" and typeof(inputTable.get) == "function"

	local self = setmetatable({
		type = "State",
		kind = "ForValues",
		dependencySet = {},
		-- if we held strong references to the dependents, then they wouldn't be
		-- able to get garbage collected when they fall out of scope
		dependentSet = setmetatable({}, WEAK_KEYS_METATABLE),
		_oldDependencySet = {},

		_processor = processor,
		_destructor = destructor,
		_inputIsState = inputIsState,

		_inputTable = inputTable,
		_outputTable = {},
		_valueCache = {},
		_oldValueCache = {},
	}, CLASS_METATABLE)

	initDependency(self)
	self:update()

	return self
end

return ForValues
end)() end,
    [45] = function()local wax,script,require=ImportGlobals(45)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs a new state object which can listen for updates on another state
	object.

	FIXME: enabling strict types here causes free types to leak
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local Types = require(Package.Types)
local initDependency = require(Package.Dependencies.initDependency)

type Set<T> = {[T]: any}

local class = {}
local CLASS_METATABLE = {__index = class}

-- Table used to hold Observer objects in memory.
local strongRefs: Set<Types.Observer> = {}

--[[
	Called when the watched state changes value.
]]
function class:update(): boolean
	for _, callback in pairs(self._changeListeners) do
		task.spawn(callback)
	end
	return false
end

--[[
	Adds a change listener. When the watched state changes value, the listener
	will be fired.

	Returns a function which, when called, will disconnect the change listener.
	As long as there is at least one active change listener, this Observer
	will be held in memory, preventing GC, so disconnecting is important.
]]
function class:onChange(callback: () -> ()): () -> ()
	local uniqueIdentifier = {}

	self._numChangeListeners += 1
	self._changeListeners[uniqueIdentifier] = callback

	-- disallow gc (this is important to make sure changes are received)
	strongRefs[self] = true

	local disconnected = false
	return function()
		if disconnected then
			return
		end
		disconnected = true
		self._changeListeners[uniqueIdentifier] = nil
		self._numChangeListeners -= 1

		if self._numChangeListeners == 0 then
			-- allow gc if all listeners are disconnected
			strongRefs[self] = nil
		end
	end
end

local function Observer(watchedState: PubTypes.Value<any>): Types.Observer
	local self = setmetatable({
		type = "State",
		kind = "Observer",
		dependencySet = {[watchedState] = true},
		dependentSet = {},
		_changeListeners = {},
		_numChangeListeners = 0,
	}, CLASS_METATABLE)

	initDependency(self)
	-- add this object to the watched state's dependent set
	watchedState.dependentSet[self] = true

	return self
end

return Observer
end)() end,
    [46] = function()local wax,script,require=ImportGlobals(46)local ImportGlobals return (function(...)--!nonstrict

--[[
	Constructs and returns objects which can be used to model independent
	reactive state.
]]

local Package = script.Parent.Parent
local Types = require(Package.Types)
local useDependency = require(Package.Dependencies.useDependency)
local initDependency = require(Package.Dependencies.initDependency)
local updateAll = require(Package.Dependencies.updateAll)
local isSimilar = require(Package.Utility.isSimilar)

local class = {}

local CLASS_METATABLE = {__index = class}
local WEAK_KEYS_METATABLE = {__mode = "k"}

--[[
	Returns the value currently stored in this State object.
	The state object will be registered as a dependency unless `asDependency` is
	false.
]]
function class:get(asDependency: boolean?): any
	if asDependency ~= false then
		useDependency(self)
	end
	return self._value
end

--[[
	Updates the value stored in this State object.

	If `force` is enabled, this will skip equality checks and always update the
	state object and any dependents - use this with care as this can lead to
	unnecessary updates.
]]
function class:set(newValue: any, force: boolean?)
	local oldValue = self._value
	if force or not isSimilar(oldValue, newValue) then
		self._value = newValue
		updateAll(self)
	end
end

local function Value<T>(initialValue: T): Types.State<T>
	local self = setmetatable({
		type = "State",
		kind = "Value",
		-- if we held strong references to the dependents, then they wouldn't be
		-- able to get garbage collected when they fall out of scope
		dependentSet = setmetatable({}, WEAK_KEYS_METATABLE),
		_value = initialValue
	}, CLASS_METATABLE)

	initDependency(self)

	return self
end

return Value
end)() end,
    [47] = function()local wax,script,require=ImportGlobals(47)local ImportGlobals return (function(...)--!strict

--[[
	A common interface for accessing the values of state objects or constants.
]]

local Package = script.Parent.Parent
local PubTypes = require(Package.PubTypes)
local xtypeof = require(Package.Utility.xtypeof)

local function unwrap<T>(item: PubTypes.CanBeState<T>, useDependency: boolean?): T
	return if xtypeof(item) == "State" then (item :: PubTypes.StateObject<T>):get(useDependency) else (item :: T)
end

return unwrap
end)() end,
    [48] = function()local wax,script,require=ImportGlobals(48)local ImportGlobals return (function(...)--!strict

--[[
	Stores common type information used internally.

	These types may be used internally so Fusion code can type-check, but
	should never be exposed to public users, as these definitions are fair game
	for breaking changes.
]]

local Package = script.Parent
local PubTypes = require(Package.PubTypes)

type Set<T> = {[T]: any}

--[[
	General use types
]]

-- A symbol that represents the absence of a value.
export type None = PubTypes.Symbol & {
	-- name: "None" (add this when Luau supports singleton types)
}

-- Stores useful information about Luau errors.
export type Error = {
	type: string, -- replace with "Error" when Luau supports singleton types
	raw: string,
	message: string,
	trace: string
}

--[[
	Specific reactive graph types
]]

-- A state object whose value can be set at any time by the user.
export type State<T> = PubTypes.Value<T> & {
	_value: T
}

-- A state object whose value is derived from other objects using a callback.
export type Computed<T> = PubTypes.Computed<T> & {
	_oldDependencySet: Set<PubTypes.Dependency>,
	_callback: () -> T,
	_value: T
}

-- A state object whose value is derived from other objects using a callback.
export type ForPairs<KI, VI, KO, VO, M> = PubTypes.ForPairs<KO, VO> & {
	_oldDependencySet: Set<PubTypes.Dependency>,
	_processor: (KI, VI) -> (KO, VO),
	_destructor: (VO, M?) -> (),
	_inputIsState: boolean,
	_inputTable: PubTypes.CanBeState<{ [KI]: VI }>,
	_oldInputTable: { [KI]: VI },
	_outputTable: { [KO]: VO },
	_oldOutputTable: { [KO]: VO },
	_keyIOMap: { [KI]: KO },
	_meta: { [KO]: M? },
	_keyData: {
		[KI]: {
			dependencySet: Set<PubTypes.Dependency>,
			oldDependencySet: Set<PubTypes.Dependency>,
			dependencyValues: { [PubTypes.Dependency]: any },
		},
	},
}

-- A state object whose value is derived from other objects using a callback.
export type ForKeys<KI, KO, M> = PubTypes.ForKeys<KO, any> & {
	_oldDependencySet: Set<PubTypes.Dependency>,
	_processor: (KI) -> (KO),
	_destructor: (KO, M?) -> (),
	_inputIsState: boolean,
	_inputTable: PubTypes.CanBeState<{ [KI]: KO }>,
	_oldInputTable: { [KI]: KO },
	_outputTable: { [KO]: any },
	_keyOIMap: { [KO]: KI },
	_meta: { [KO]: M? },
	_keyData: {
		[KI]: {
			dependencySet: Set<PubTypes.Dependency>,
			oldDependencySet: Set<PubTypes.Dependency>,
			dependencyValues: { [PubTypes.Dependency]: any },
		},
	},
}

-- A state object whose value is derived from other objects using a callback.
export type ForValues<VI, VO, M> = PubTypes.ForValues<any, VO> & {
	_oldDependencySet: Set<PubTypes.Dependency>,
	_processor: (VI) -> (VO),
	_destructor: (VO, M?) -> (),
	_inputIsState: boolean,
	_inputTable: PubTypes.CanBeState<{ [VI]: VO }>,
	_outputTable: { [any]: VI },
	_valueCache: { [VO]: any },
	_oldValueCache: { [VO]: any },
	_meta: { [VO]: M? },
	_valueData: {
		[VI]: {
			dependencySet: Set<PubTypes.Dependency>,
			oldDependencySet: Set<PubTypes.Dependency>,
			dependencyValues: { [PubTypes.Dependency]: any },
		},
	},
}

-- A state object which follows another state object using tweens.
export type Tween<T> = PubTypes.Tween<T> & {
	_goalState: State<T>,
	_tweenInfo: TweenInfo,
	_prevValue: T,
	_nextValue: T,
	_currentValue: T,
	_currentTweenInfo: TweenInfo,
	_currentTweenDuration: number,
	_currentTweenStartTime: number,
	_currentlyAnimating: boolean
}

-- A state object which follows another state object using spring simulation.
export type Spring<T> = PubTypes.Spring<T> & {
	_speed: PubTypes.CanBeState<number>,
	_speedIsState: boolean,
	_lastSpeed: number,
	_damping: PubTypes.CanBeState<number>,
	_dampingIsState: boolean,
	_lastDamping: number,
	_goalState: State<T>,
	_goalValue: T,
	_currentType: string,
	_currentValue: T,
	_springPositions: {number},
	_springGoals: {number},
	_springVelocities: {number}
}

-- An object which can listen for updates on another state object.
export type Observer = PubTypes.Observer & {
	_changeListeners: Set<() -> ()>,
	_numChangeListeners: number
}

return nil
end)() end,
    [50] = function()local wax,script,require=ImportGlobals(50)local ImportGlobals return (function(...)--!strict

--[[
	A symbol for representing nil values in contexts where nil is not usable.
]]

local Package = script.Parent.Parent
local Types = require(Package.Types)

return {
	type = "Symbol",
	name = "None"
} :: Types.None
end)() end,
    [51] = function()local wax,script,require=ImportGlobals(51)local ImportGlobals return (function(...)--!strict

--[[
	Cleans up the tasks passed in as the arguments.
	A task can be any of the following:

	- an Instance - will be destroyed
	- an RBXScriptConnection - will be disconnected
	- a function - will be run
	- a table with a `Destroy` or `destroy` function - will be called
	- an array - `cleanup` will be called on each item
]]

local function cleanupOne(task: any)
	local taskType = typeof(task)

	-- case 1: Instance
	if taskType == "Instance" then
		task:Destroy()

	-- case 2: RBXScriptConnection
	elseif taskType == "RBXScriptConnection" then
		task:Disconnect()

	-- case 3: callback
	elseif taskType == "function" then
		task()

	elseif taskType == "table" then
		-- case 4: destroy() function
		if typeof(task.destroy) == "function" then
			task:destroy()

		-- case 5: Destroy() function
		elseif typeof(task.Destroy) == "function" then
			task:Destroy()

		-- case 6: array of tasks
		elseif task[1] ~= nil then
			for _, subtask in ipairs(task) do
				cleanupOne(subtask)
			end
		end
	end
end

local function cleanup(...: any)
	for index = 1, select("#", ...) do
		cleanupOne(select(index, ...))
	end
end

return cleanup
end)() end,
    [52] = function()local wax,script,require=ImportGlobals(52)local ImportGlobals return (function(...)--!strict

--[[
	An empty function. Often used as a destructor to indicate no destruction.
]]

local function doNothing(...: any)
end

return doNothing
end)() end,
    [53] = function()local wax,script,require=ImportGlobals(53)local ImportGlobals return (function(...)--!strict
--[[
    Returns true if A and B are 'similar' - i.e. any user of A would not need
    to recompute if it changed to B.
]]

local function isSimilar(a: any, b: any): boolean
    -- HACK: because tables are mutable data structures, don't make assumptions
    -- about similarity from equality for now (see issue #44)
    if typeof(a) == "table" then
        return false
    else
        return a == b
    end
end

return isSimilar
end)() end,
    [54] = function()local wax,script,require=ImportGlobals(54)local ImportGlobals return (function(...)--!strict

--[[
    Returns true if the given value is not automatically memory managed, and
    requires manual cleanup.
]]

local function needsDestruction(x: any): boolean
    return typeof(x) == "Instance"
end

return needsDestruction
end)() end,
    [55] = function()local wax,script,require=ImportGlobals(55)local ImportGlobals return (function(...)--!strict

--[[
	Restricts the reading of missing members for a table.
]]

local Package = script.Parent.Parent
local logError = require(Package.Logging.logError)

type table = {[any]: any}

local function restrictRead(tableName: string, strictTable: table): table
	-- FIXME: Typed Luau doesn't recognise this correctly yet
	local metatable = getmetatable(strictTable :: any)

	if metatable == nil then
		metatable = {}
		setmetatable(strictTable, metatable)
	end

	function metatable:__index(memberName)
		logError("strictReadError", nil, tostring(memberName), tableName)
	end

	return strictTable
end

return restrictRead
end)() end,
    [56] = function()local wax,script,require=ImportGlobals(56)local ImportGlobals return (function(...)--!strict

--[[
	Extended typeof, designed for identifying custom objects.
	If given a table with a `type` string, returns that.
	Otherwise, returns `typeof()` the argument.
]]

local function xtypeof(x: any)
	local typeString = typeof(x)

	if typeString == "table" and typeof(x.type) == "string" then
		return x.type
	else
		return typeString
	end
end

return xtypeof
end)() end,
    [57] = function()local wax,script,require=ImportGlobals(57)local ImportGlobals return (function(...)local randomString = require(script.Parent.Parent.utilities.randomString)
local fusion = require(script.Parent.Fusion)
local value = fusion.Value

local data = {}
local dataFunctions = {}

type uid = string
type tag = string

function dataFunctions.update(uid:string, index:string, new:string)
    if index ~= "uid" and dataFunctions.find(uid) and dataFunctions.find(uid)[index] then
        local dataToEdit = dataFunctions.find(uid)
        dataToEdit[index]:set(new)
    elseif not dataFunctions.find(uid) then
        error("couldn't find uid: "..uid)
    elseif index == "uid" or index == "tag" then
        error("cannot edit property "..index)
    elseif not dataFunctions.find(uid)[index] then
        error("index returned nil")
    else
        error("how?")
    end
end
function dataFunctions.find(identifier:uid | tag):table
    local returnData = nil
    for i=1, #data do
        if data[i].uid == identifier or data[i].tag == identifier then
            returnData = data[i]
        end
    end
    if not returnData then
        warn("no data found by identifier: "..tostring(identifier))
        return returnData
    end
    return returnData
end
function dataFunctions.delete(uid:string)
    local dataToDelete = dataFunctions.find(uid)
    local index = table.find(data, dataToDelete)
    local newData = {}
    for i=1, #data do
        if i ~= index then
            table.insert(newData, data[i])
        end
    end
    data = newData
    newData =  nil
end
function dataFunctions.add(newData:table, tag:string):table
    for i,v in newData do
        newData[i] = value(v)
    end
    newData.tag = tag or nil
    newData.uid = randomString(16)
    setmetatable(newData, {
        __index = {
            ["update"] = function(tbl, dataType, info)
                dataFunctions.update(tbl.uid, dataType, info)
            end,
            ["delete"] = function()
                dataFunctions.delete(newData.uid)
            end
        }
    })
    table.insert(data, newData)
    return dataFunctions.find(newData.uid)
end

return dataFunctions

end)() end,
    [58] = function()local wax,script,require=ImportGlobals(58)local ImportGlobals return (function(...)return {
    ['lock'] = "rbxassetid://10723434711",
    ['dropdown'] = "rbxassetid://15555233897",
    ['chevron'] = "rbxassetid://10709790948",
    ['chevronRight'] = "rbxassetid://10709791437",
    ['emptyBox'] = "rbxassetid://15555208034",
    ['filledBox'] = "rbxassetid://15555206993",
    ['maximize'] = "rbxassetid://15556636376",
    ['minimize'] = "rbxassetid://15556637715",
    ['close'] = "rbxassetid://10747384394",
    ['circle'] = "rbxassetid://18804307590",
    ['click'] = "rbxassetid://10734898355",
    ['normalDropdown'] = "rbxassetid://10723433811",
    ['multiDropdown'] = "rbxassetid://10734884548"
}
end)() end,
    [59] = function()local wax,script,require=ImportGlobals(59)local ImportGlobals return (function(...)--[[ This file was @generated by Tarmac. It is not intended for manual editing.]]
local assets = {
	["accessibility"] = "rbxassetid://10709751939",
	["activity"] = "rbxassetid://10709752035",
	["air-vent"] = "rbxassetid://10709752131",
	["airplay"] = "rbxassetid://10709752254",
	["alarm-check"] = "rbxassetid://10709752405",
	["alarm-clock"] = "rbxassetid://10709752630",
	["alarm-clock-off"] = "rbxassetid://10709752508",
	["alarm-minus"] = "rbxassetid://10709752732",
	["alarm-plus"] = "rbxassetid://10709752825",
	["album"] = "rbxassetid://10709752906",
	["alert-circle"] = "rbxassetid://10709752996",
	["alert-octagon"] = "rbxassetid://10709753064",
	["alert-triangle"] = "rbxassetid://10709753149",
	["align-center"] = "rbxassetid://10709753570",
	["align-center-horizontal"] = "rbxassetid://10709753272",
	["align-center-vertical"] = "rbxassetid://10709753421",
	["align-end-horizontal"] = "rbxassetid://10709753692",
	["align-end-vertical"] = "rbxassetid://10709753808",
	["align-horizontal-distribute-center"] = "rbxassetid://10747779791",
	["align-horizontal-distribute-end"] = "rbxassetid://10747784534",
	["align-horizontal-distribute-start"] = "rbxassetid://10709754118",
	["align-horizontal-justify-center"] = "rbxassetid://10709754204",
	["align-horizontal-justify-end"] = "rbxassetid://10709754317",
	["align-horizontal-justify-start"] = "rbxassetid://10709754436",
	["align-horizontal-space-around"] = "rbxassetid://10709754590",
	["align-horizontal-space-between"] = "rbxassetid://10709754749",
	["align-justify"] = "rbxassetid://10709759610",
	["align-left"] = "rbxassetid://10709759764",
	["align-right"] = "rbxassetid://10709759895",
	["align-start-horizontal"] = "rbxassetid://10709760051",
	["align-start-vertical"] = "rbxassetid://10709760244",
	["align-vertical-distribute-center"] = "rbxassetid://10709760351",
	["align-vertical-distribute-end"] = "rbxassetid://10709760434",
	["align-vertical-distribute-start"] = "rbxassetid://10709760612",
	["align-vertical-justify-center"] = "rbxassetid://10709760814",
	["align-vertical-justify-end"] = "rbxassetid://10709761003",
	["align-vertical-justify-start"] = "rbxassetid://10709761176",
	["align-vertical-space-around"] = "rbxassetid://10709761324",
	["align-vertical-space-between"] = "rbxassetid://10709761434",
	["anchor"] = "rbxassetid://10709761530",
	["angry"] = "rbxassetid://10709761629",
	["annoyed"] = "rbxassetid://10709761722",
	["aperture"] = "rbxassetid://10709761813",
	["apple"] = "rbxassetid://10709761889",
	["archive"] = "rbxassetid://10709762233",
	["archive-restore"] = "rbxassetid://10709762058",
	["armchair"] = "rbxassetid://10709762327",
	["arrow-big-down"] = "rbxassetid://10747796644",
	["arrow-big-left"] = "rbxassetid://10709762574",
	["arrow-big-right"] = "rbxassetid://10709762727",
	["arrow-big-up"] = "rbxassetid://10709762879",
	["arrow-down"] = "rbxassetid://10709767827",
	["arrow-down-circle"] = "rbxassetid://10709763034",
	["arrow-down-left"] = "rbxassetid://10709767656",
	["arrow-down-right"] = "rbxassetid://10709767750",
	["arrow-left"] = "rbxassetid://10709768114",
	["arrow-left-circle"] = "rbxassetid://10709767936",
	["arrow-left-right"] = "rbxassetid://10709768019",
	["arrow-right"] = "rbxassetid://10709768347",
	["arrow-right-circle"] = "rbxassetid://10709768226",
	["arrow-up"] = "rbxassetid://10709768939",
	["arrow-up-circle"] = "rbxassetid://10709768432",
	["arrow-up-down"] = "rbxassetid://10709768538",
	["arrow-up-left"] = "rbxassetid://10709768661",
	["arrow-up-right"] = "rbxassetid://10709768787",
	["asterisk"] = "rbxassetid://10709769095",
	["at-sign"] = "rbxassetid://10709769286",
	["award"] = "rbxassetid://10709769406",
	["axe"] = "rbxassetid://10709769508",
	["axis-3d"] = "rbxassetid://10709769598",
	["baby"] = "rbxassetid://10709769732",
	["backpack"] = "rbxassetid://10709769841",
	["baggage-claim"] = "rbxassetid://10709769935",
	["banana"] = "rbxassetid://10709770005",
	["banknote"] = "rbxassetid://10709770178",
	["bar-chart"] = "rbxassetid://10709773755",
	["bar-chart-2"] = "rbxassetid://10709770317",
	["bar-chart-3"] = "rbxassetid://10709770431",
	["bar-chart-4"] = "rbxassetid://10709770560",
	["bar-chart-horizontal"] = "rbxassetid://10709773669",
	["barcode"] = "rbxassetid://10747360675",
	["baseline"] = "rbxassetid://10709773863",
	["bath"] = "rbxassetid://10709773963",
	["battery"] = "rbxassetid://10709774640",
	["battery-charging"] = "rbxassetid://10709774068",
	["battery-full"] = "rbxassetid://10709774206",
	["battery-low"] = "rbxassetid://10709774370",
	["battery-medium"] = "rbxassetid://10709774513",
	["beaker"] = "rbxassetid://10709774756",
	["bed"] = "rbxassetid://10709775036",
	["bed-double"] = "rbxassetid://10709774864",
	["bed-single"] = "rbxassetid://10709774968",
	["beer"] = "rbxassetid://10709775167",
	["bell"] = "rbxassetid://10709775704",
	["bell-minus"] = "rbxassetid://10709775241",
	["bell-off"] = "rbxassetid://10709775320",
	["bell-plus"] = "rbxassetid://10709775448",
	["bell-ring"] = "rbxassetid://10709775560",
	["bike"] = "rbxassetid://10709775894",
	["binary"] = "rbxassetid://10709776050",
	["bitcoin"] = "rbxassetid://10709776126",
	["bluetooth"] = "rbxassetid://10709776655",
	["bluetooth-connected"] = "rbxassetid://10709776240",
	["bluetooth-off"] = "rbxassetid://10709776344",
	["bluetooth-searching"] = "rbxassetid://10709776501",
	["bold"] = "rbxassetid://10747813908",
	["bomb"] = "rbxassetid://10709781460",
	["bone"] = "rbxassetid://10709781605",
	["book"] = "rbxassetid://10709781824",
	["book-open"] = "rbxassetid://10709781717",
	["bookmark"] = "rbxassetid://10709782154",
	["bookmark-minus"] = "rbxassetid://10709781919",
	["bookmark-plus"] = "rbxassetid://10709782044",
	["bot"] = "rbxassetid://10709782230",
	["box"] = "rbxassetid://10709782497",
	["box-select"] = "rbxassetid://10709782342",
	["boxes"] = "rbxassetid://10709782582",
	["briefcase"] = "rbxassetid://10709782662",
	["brush"] = "rbxassetid://10709782758",
	["bug"] = "rbxassetid://10709782845",
	["building"] = "rbxassetid://10709783051",
	["building-2"] = "rbxassetid://10709782939",
	["bus"] = "rbxassetid://10709783137",
	["cake"] = "rbxassetid://10709783217",
	["calculator"] = "rbxassetid://10709783311",
	["calendar"] = "rbxassetid://10709789505",
	["calendar-check"] = "rbxassetid://10709783474",
	["calendar-check-2"] = "rbxassetid://10709783392",
	["calendar-clock"] = "rbxassetid://10709783577",
	["calendar-days"] = "rbxassetid://10709783673",
	["calendar-heart"] = "rbxassetid://10709783835",
	["calendar-minus"] = "rbxassetid://10709783959",
	["calendar-off"] = "rbxassetid://10709788784",
	["calendar-plus"] = "rbxassetid://10709788937",
	["calendar-range"] = "rbxassetid://10709789053",
	["calendar-search"] = "rbxassetid://10709789200",
	["calendar-x"] = "rbxassetid://10709789407",
	["calendar-x-2"] = "rbxassetid://10709789329",
	["camera"] = "rbxassetid://10709789686",
	["camera-off"] = "rbxassetid://10747822677",
	["car"] = "rbxassetid://10709789810",
	["carrot"] = "rbxassetid://10709789960",
	["cast"] = "rbxassetid://10709790097",
	["charge"] = "rbxassetid://10709790202",
	["check"] = "rbxassetid://10709790644",
	["check-circle"] = "rbxassetid://10709790387",
	["check-circle-2"] = "rbxassetid://10709790298",
	["check-square"] = "rbxassetid://10709790537",
	["chef-hat"] = "rbxassetid://10709790757",
	["cherry"] = "rbxassetid://10709790875",
	["chevron-down"] = "rbxassetid://10709790948",
	["chevron-first"] = "rbxassetid://10709791015",
	["chevron-last"] = "rbxassetid://10709791130",
	["chevron-left"] = "rbxassetid://10709791281",
	["chevron-right"] = "rbxassetid://10709791437",
	["chevron-up"] = "rbxassetid://10709791523",
	["chevrons-down"] = "rbxassetid://10709796864",
	["chevrons-down-up"] = "rbxassetid://10709791632",
	["chevrons-left"] = "rbxassetid://10709797151",
	["chevrons-left-right"] = "rbxassetid://10709797006",
	["chevrons-right"] = "rbxassetid://10709797382",
	["chevrons-right-left"] = "rbxassetid://10709797274",
	["chevrons-up"] = "rbxassetid://10709797622",
	["chevrons-up-down"] = "rbxassetid://10709797508",
	["chrome"] = "rbxassetid://10709797725",
	["circle"] = "rbxassetid://10709798174",
	["circle-dot"] = "rbxassetid://10709797837",
	["circle-ellipsis"] = "rbxassetid://10709797985",
	["circle-slashed"] = "rbxassetid://10709798100",
	["citrus"] = "rbxassetid://10709798276",
	["clapperboard"] = "rbxassetid://10709798350",
	["clipboard"] = "rbxassetid://10709799288",
	["clipboard-check"] = "rbxassetid://10709798443",
	["clipboard-copy"] = "rbxassetid://10709798574",
	["clipboard-edit"] = "rbxassetid://10709798682",
	["clipboard-list"] = "rbxassetid://10709798792",
	["clipboard-signature"] = "rbxassetid://10709798890",
	["clipboard-type"] = "rbxassetid://10709798999",
	["clipboard-x"] = "rbxassetid://10709799124",
	["clock"] = "rbxassetid://10709805144",
	["clock-1"] = "rbxassetid://10709799535",
	["clock-10"] = "rbxassetid://10709799718",
	["clock-11"] = "rbxassetid://10709799818",
	["clock-12"] = "rbxassetid://10709799962",
	["clock-2"] = "rbxassetid://10709803876",
	["clock-3"] = "rbxassetid://10709803989",
	["clock-4"] = "rbxassetid://10709804164",
	["clock-5"] = "rbxassetid://10709804291",
	["clock-6"] = "rbxassetid://10709804435",
	["clock-7"] = "rbxassetid://10709804599",
	["clock-8"] = "rbxassetid://10709804784",
	["clock-9"] = "rbxassetid://10709804996",
	["cloud"] = "rbxassetid://10709806740",
	["cloud-cog"] = "rbxassetid://10709805262",
	["cloud-drizzle"] = "rbxassetid://10709805371",
	["cloud-fog"] = "rbxassetid://10709805477",
	["cloud-hail"] = "rbxassetid://10709805596",
	["cloud-lightning"] = "rbxassetid://10709805727",
	["cloud-moon"] = "rbxassetid://10709805942",
	["cloud-moon-rain"] = "rbxassetid://10709805838",
	["cloud-off"] = "rbxassetid://10709806060",
	["cloud-rain"] = "rbxassetid://10709806277",
	["cloud-rain-wind"] = "rbxassetid://10709806166",
	["cloud-snow"] = "rbxassetid://10709806374",
	["cloud-sun"] = "rbxassetid://10709806631",
	["cloud-sun-rain"] = "rbxassetid://10709806475",
	["cloudy"] = "rbxassetid://10709806859",
	["clover"] = "rbxassetid://10709806995",
	["code"] = "rbxassetid://10709810463",
	["code-2"] = "rbxassetid://10709807111",
	["codepen"] = "rbxassetid://10709810534",
	["codesandbox"] = "rbxassetid://10709810676",
	["coffee"] = "rbxassetid://10709810814",
	["cog"] = "rbxassetid://10709810948",
	["coins"] = "rbxassetid://10709811110",
	["columns"] = "rbxassetid://10709811261",
	["command"] = "rbxassetid://10709811365",
	["compass"] = "rbxassetid://10709811445",
	["component"] = "rbxassetid://10709811595",
	["concierge-bell"] = "rbxassetid://10709811706",
	["connection"] = "rbxassetid://10747361219",
	["contact"] = "rbxassetid://10709811834",
	["contrast"] = "rbxassetid://10709811939",
	["cookie"] = "rbxassetid://10709812067",
	["copy"] = "rbxassetid://10709812159",
	["copyleft"] = "rbxassetid://10709812251",
	["copyright"] = "rbxassetid://10709812311",
	["corner-down-left"] = "rbxassetid://10709812396",
	["corner-down-right"] = "rbxassetid://10709812485",
	["corner-left-down"] = "rbxassetid://10709812632",
	["corner-left-up"] = "rbxassetid://10709812784",
	["corner-right-down"] = "rbxassetid://10709812939",
	["corner-right-up"] = "rbxassetid://10709813094",
	["corner-up-left"] = "rbxassetid://10709813185",
	["corner-up-right"] = "rbxassetid://10709813281",
	["cpu"] = "rbxassetid://10709813383",
	["croissant"] = "rbxassetid://10709818125",
	["crop"] = "rbxassetid://10709818245",
	["cross"] = "rbxassetid://10709818399",
	["crosshair"] = "rbxassetid://10709818534",
	["crown"] = "rbxassetid://10709818626",
	["cup-soda"] = "rbxassetid://10709818763",
	["curly-braces"] = "rbxassetid://10709818847",
	["currency"] = "rbxassetid://10709818931",
	["database"] = "rbxassetid://10709818996",
	["delete"] = "rbxassetid://10709819059",
	["diamond"] = "rbxassetid://10709819149",
	["dice-1"] = "rbxassetid://10709819266",
	["dice-2"] = "rbxassetid://10709819361",
	["dice-3"] = "rbxassetid://10709819508",
	["dice-4"] = "rbxassetid://10709819670",
	["dice-5"] = "rbxassetid://10709819801",
	["dice-6"] = "rbxassetid://10709819896",
	["dices"] = "rbxassetid://10723343321",
	["diff"] = "rbxassetid://10723343416",
	["disc"] = "rbxassetid://10723343537",
	["divide"] = "rbxassetid://10723343805",
	["divide-circle"] = "rbxassetid://10723343636",
	["divide-square"] = "rbxassetid://10723343737",
	["dollar-sign"] = "rbxassetid://10723343958",
	["download"] = "rbxassetid://10723344270",
	["download-cloud"] = "rbxassetid://10723344088",
	["droplet"] = "rbxassetid://10723344432",
	["droplets"] = "rbxassetid://10734883356",
	["drumstick"] = "rbxassetid://10723344737",
	["edit"] = "rbxassetid://10734883598",
	["edit-2"] = "rbxassetid://10723344885",
	["edit-3"] = "rbxassetid://10723345088",
	["egg"] = "rbxassetid://10723345518",
	["egg-fried"] = "rbxassetid://10723345347",
	["electricity"] = "rbxassetid://10723345749",
	["electricity-off"] = "rbxassetid://10723345643",
	["equal"] = "rbxassetid://10723345990",
	["equal-not"] = "rbxassetid://10723345866",
	["eraser"] = "rbxassetid://10723346158",
	["euro"] = "rbxassetid://10723346372",
	["expand"] = "rbxassetid://10723346553",
	["external-link"] = "rbxassetid://10723346684",
	["eye"] = "rbxassetid://10723346959",
	["eye-off"] = "rbxassetid://10723346871",
	["factory"] = "rbxassetid://10723347051",
	["fan"] = "rbxassetid://10723354359",
	["fast-forward"] = "rbxassetid://10723354521",
	["feather"] = "rbxassetid://10723354671",
	["figma"] = "rbxassetid://10723354801",
	["file"] = "rbxassetid://10723374641",
	["file-archive"] = "rbxassetid://10723354921",
	["file-audio"] = "rbxassetid://10723355148",
	["file-audio-2"] = "rbxassetid://10723355026",
	["file-axis-3d"] = "rbxassetid://10723355272",
	["file-badge"] = "rbxassetid://10723355622",
	["file-badge-2"] = "rbxassetid://10723355451",
	["file-bar-chart"] = "rbxassetid://10723355887",
	["file-bar-chart-2"] = "rbxassetid://10723355746",
	["file-box"] = "rbxassetid://10723355989",
	["file-check"] = "rbxassetid://10723356210",
	["file-check-2"] = "rbxassetid://10723356100",
	["file-clock"] = "rbxassetid://10723356329",
	["file-code"] = "rbxassetid://10723356507",
	["file-cog"] = "rbxassetid://10723356830",
	["file-cog-2"] = "rbxassetid://10723356676",
	["file-diff"] = "rbxassetid://10723357039",
	["file-digit"] = "rbxassetid://10723357151",
	["file-down"] = "rbxassetid://10723357322",
	["file-edit"] = "rbxassetid://10723357495",
	["file-heart"] = "rbxassetid://10723357637",
	["file-image"] = "rbxassetid://10723357790",
	["file-input"] = "rbxassetid://10723357933",
	["file-json"] = "rbxassetid://10723364435",
	["file-json-2"] = "rbxassetid://10723364361",
	["file-key"] = "rbxassetid://10723364605",
	["file-key-2"] = "rbxassetid://10723364515",
	["file-line-chart"] = "rbxassetid://10723364725",
	["file-lock"] = "rbxassetid://10723364957",
	["file-lock-2"] = "rbxassetid://10723364861",
	["file-minus"] = "rbxassetid://10723365254",
	["file-minus-2"] = "rbxassetid://10723365086",
	["file-output"] = "rbxassetid://10723365457",
	["file-pie-chart"] = "rbxassetid://10723365598",
	["file-plus"] = "rbxassetid://10723365877",
	["file-plus-2"] = "rbxassetid://10723365766",
	["file-question"] = "rbxassetid://10723365987",
	["file-scan"] = "rbxassetid://10723366167",
	["file-search"] = "rbxassetid://10723366550",
	["file-search-2"] = "rbxassetid://10723366340",
	["file-signature"] = "rbxassetid://10723366741",
	["file-spreadsheet"] = "rbxassetid://10723366962",
	["file-symlink"] = "rbxassetid://10723367098",
	["file-terminal"] = "rbxassetid://10723367244",
	["file-text"] = "rbxassetid://10723367380",
	["file-type"] = "rbxassetid://10723367606",
	["file-type-2"] = "rbxassetid://10723367509",
	["file-up"] = "rbxassetid://10723367734",
	["file-video"] = "rbxassetid://10723373884",
	["file-video-2"] = "rbxassetid://10723367834",
	["file-volume"] = "rbxassetid://10723374172",
	["file-volume-2"] = "rbxassetid://10723374030",
	["file-warning"] = "rbxassetid://10723374276",
	["file-x"] = "rbxassetid://10723374544",
	["file-x-2"] = "rbxassetid://10723374378",
	["files"] = "rbxassetid://10723374759",
	["film"] = "rbxassetid://10723374981",
	["filter"] = "rbxassetid://10723375128",
	["fingerprint"] = "rbxassetid://10723375250",
	["flag"] = "rbxassetid://10723375890",
	["flag-off"] = "rbxassetid://10723375443",
	["flag-triangle-left"] = "rbxassetid://10723375608",
	["flag-triangle-right"] = "rbxassetid://10723375727",
	["flame"] = "rbxassetid://10723376114",
	["flashlight"] = "rbxassetid://10723376471",
	["flashlight-off"] = "rbxassetid://10723376365",
	["flask-conical"] = "rbxassetid://10734883986",
	["flask-round"] = "rbxassetid://10723376614",
	["flip-horizontal"] = "rbxassetid://10723376884",
	["flip-horizontal-2"] = "rbxassetid://10723376745",
	["flip-vertical"] = "rbxassetid://10723377138",
	["flip-vertical-2"] = "rbxassetid://10723377026",
	["flower"] = "rbxassetid://10747830374",
	["flower-2"] = "rbxassetid://10723377305",
	["focus"] = "rbxassetid://10723377537",
	["folder"] = "rbxassetid://10723387563",
	["folder-archive"] = "rbxassetid://10723384478",
	["folder-check"] = "rbxassetid://10723384605",
	["folder-clock"] = "rbxassetid://10723384731",
	["folder-closed"] = "rbxassetid://10723384893",
	["folder-cog"] = "rbxassetid://10723385213",
	["folder-cog-2"] = "rbxassetid://10723385036",
	["folder-down"] = "rbxassetid://10723385338",
	["folder-edit"] = "rbxassetid://10723385445",
	["folder-heart"] = "rbxassetid://10723385545",
	["folder-input"] = "rbxassetid://10723385721",
	["folder-key"] = "rbxassetid://10723385848",
	["folder-lock"] = "rbxassetid://10723386005",
	["folder-minus"] = "rbxassetid://10723386127",
	["folder-open"] = "rbxassetid://10723386277",
	["folder-output"] = "rbxassetid://10723386386",
	["folder-plus"] = "rbxassetid://10723386531",
	["folder-search"] = "rbxassetid://10723386787",
	["folder-search-2"] = "rbxassetid://10723386674",
	["folder-symlink"] = "rbxassetid://10723386930",
	["folder-tree"] = "rbxassetid://10723387085",
	["folder-up"] = "rbxassetid://10723387265",
	["folder-x"] = "rbxassetid://10723387448",
	["folders"] = "rbxassetid://10723387721",
	["form-input"] = "rbxassetid://10723387841",
	["forward"] = "rbxassetid://10723388016",
	["frame"] = "rbxassetid://10723394389",
	["framer"] = "rbxassetid://10723394565",
	["frown"] = "rbxassetid://10723394681",
	["fuel"] = "rbxassetid://10723394846",
	["function-square"] = "rbxassetid://10723395041",
	["gamepad"] = "rbxassetid://10723395457",
	["gamepad-2"] = "rbxassetid://10723395215",
	["gauge"] = "rbxassetid://10723395708",
	["gavel"] = "rbxassetid://10723395896",
	["gem"] = "rbxassetid://10723396000",
	["ghost"] = "rbxassetid://10723396107",
	["gift"] = "rbxassetid://10723396402",
	["gift-card"] = "rbxassetid://10723396225",
	["git-branch"] = "rbxassetid://10723396676",
	["git-branch-plus"] = "rbxassetid://10723396542",
	["git-commit"] = "rbxassetid://10723396812",
	["git-compare"] = "rbxassetid://10723396954",
	["git-fork"] = "rbxassetid://10723397049",
	["git-merge"] = "rbxassetid://10723397165",
	["git-pull-request"] = "rbxassetid://10723397431",
	["git-pull-request-closed"] = "rbxassetid://10723397268",
	["git-pull-request-draft"] = "rbxassetid://10734884302",
	["glass"] = "rbxassetid://10723397788",
	["glass-2"] = "rbxassetid://10723397529",
	["glass-water"] = "rbxassetid://10723397678",
	["glasses"] = "rbxassetid://10723397895",
	["globe"] = "rbxassetid://10723404337",
	["globe-2"] = "rbxassetid://10723398002",
	["grab"] = "rbxassetid://10723404472",
	["graduation-cap"] = "rbxassetid://10723404691",
	["grape"] = "rbxassetid://10723404822",
	["grid"] = "rbxassetid://10723404936",
	["grip-horizontal"] = "rbxassetid://10723405089",
	["grip-vertical"] = "rbxassetid://10723405236",
	["hammer"] = "rbxassetid://10723405360",
	["hand"] = "rbxassetid://10723405649",
	["hand-metal"] = "rbxassetid://10723405508",
	["hard-drive"] = "rbxassetid://10723405749",
	["hard-hat"] = "rbxassetid://10723405859",
	["hash"] = "rbxassetid://10723405975",
	["haze"] = "rbxassetid://10723406078",
	["headphones"] = "rbxassetid://10723406165",
	["heart"] = "rbxassetid://10723406885",
	["heart-crack"] = "rbxassetid://10723406299",
	["heart-handshake"] = "rbxassetid://10723406480",
	["heart-off"] = "rbxassetid://10723406662",
	["heart-pulse"] = "rbxassetid://10723406795",
	["help-circle"] = "rbxassetid://10723406988",
	["hexagon"] = "rbxassetid://10723407092",
	["highlighter"] = "rbxassetid://10723407192",
	["history"] = "rbxassetid://10723407335",
	["home"] = "rbxassetid://10723407389",
	["hourglass"] = "rbxassetid://10723407498",
	["ice-cream"] = "rbxassetid://10723414308",
	["image"] = "rbxassetid://10723415040",
	["image-minus"] = "rbxassetid://10723414487",
	["image-off"] = "rbxassetid://10723414677",
	["image-plus"] = "rbxassetid://10723414827",
	["import"] = "rbxassetid://10723415205",
	["inbox"] = "rbxassetid://10723415335",
	["indent"] = "rbxassetid://10723415494",
	["indian-rupee"] = "rbxassetid://10723415642",
	["infinity"] = "rbxassetid://10723415766",
	["info"] = "rbxassetid://10723415903",
	["inspect"] = "rbxassetid://10723416057",
	["italic"] = "rbxassetid://10723416195",
	["japanese-yen"] = "rbxassetid://10723416363",
	["joystick"] = "rbxassetid://10723416527",
	["key"] = "rbxassetid://10723416652",
	["keyboard"] = "rbxassetid://10723416765",
	["lamp"] = "rbxassetid://10723417513",
	["lamp-ceiling"] = "rbxassetid://10723416922",
	["lamp-desk"] = "rbxassetid://10723417016",
	["lamp-floor"] = "rbxassetid://10723417131",
	["lamp-wall-down"] = "rbxassetid://10723417240",
	["lamp-wall-up"] = "rbxassetid://10723417356",
	["landmark"] = "rbxassetid://10723417608",
	["languages"] = "rbxassetid://10723417703",
	["laptop"] = "rbxassetid://10723423881",
	["laptop-2"] = "rbxassetid://10723417797",
	["lasso"] = "rbxassetid://10723424235",
	["lasso-select"] = "rbxassetid://10723424058",
	["laugh"] = "rbxassetid://10723424372",
	["layers"] = "rbxassetid://10723424505",
	["layout"] = "rbxassetid://10723425376",
	["layout-dashboard"] = "rbxassetid://10723424646",
	["layout-grid"] = "rbxassetid://10723424838",
	["layout-list"] = "rbxassetid://10723424963",
	["layout-template"] = "rbxassetid://10723425187",
	["leaf"] = "rbxassetid://10723425539",
	["library"] = "rbxassetid://10723425615",
	["life-buoy"] = "rbxassetid://10723425685",
	["lightbulb"] = "rbxassetid://10723425852",
	["lightbulb-off"] = "rbxassetid://10723425762",
	["line-chart"] = "rbxassetid://10723426393",
	["link"] = "rbxassetid://10723426722",
	["link-2"] = "rbxassetid://10723426595",
	["link-2-off"] = "rbxassetid://10723426513",
	["list"] = "rbxassetid://10723433811",
	["list-checks"] = "rbxassetid://10734884548",
	["list-end"] = "rbxassetid://10723426886",
	["list-minus"] = "rbxassetid://10723426986",
	["list-music"] = "rbxassetid://10723427081",
	["list-ordered"] = "rbxassetid://10723427199",
	["list-plus"] = "rbxassetid://10723427334",
	["list-start"] = "rbxassetid://10723427494",
	["list-video"] = "rbxassetid://10723427619",
	["list-x"] = "rbxassetid://10723433655",
	["loader"] = "rbxassetid://10723434070",
	["loader-2"] = "rbxassetid://10723433935",
	["locate"] = "rbxassetid://10723434557",
	["locate-fixed"] = "rbxassetid://10723434236",
	["locate-off"] = "rbxassetid://10723434379",
	["lock"] = "rbxassetid://10723434711",
	["log-in"] = "rbxassetid://10723434830",
	["log-out"] = "rbxassetid://10723434906",
	["luggage"] = "rbxassetid://10723434993",
	["magnet"] = "rbxassetid://10723435069",
	["mail"] = "rbxassetid://10734885430",
	["mail-check"] = "rbxassetid://10723435182",
	["mail-minus"] = "rbxassetid://10723435261",
	["mail-open"] = "rbxassetid://10723435342",
	["mail-plus"] = "rbxassetid://10723435443",
	["mail-question"] = "rbxassetid://10723435515",
	["mail-search"] = "rbxassetid://10734884739",
	["mail-warning"] = "rbxassetid://10734885015",
	["mail-x"] = "rbxassetid://10734885247",
	["mails"] = "rbxassetid://10734885614",
	["map"] = "rbxassetid://10734886202",
	["map-pin"] = "rbxassetid://10734886004",
	["map-pin-off"] = "rbxassetid://10734885803",
	["maximize"] = "rbxassetid://10734886735",
	["maximize-2"] = "rbxassetid://10734886496",
	["medal"] = "rbxassetid://10734887072",
	["megaphone"] = "rbxassetid://10734887454",
	["megaphone-off"] = "rbxassetid://10734887311",
	["meh"] = "rbxassetid://10734887603",
	["menu"] = "rbxassetid://10734887784",
	["message-circle"] = "rbxassetid://10734888000",
	["message-square"] = "rbxassetid://10734888228",
	["mic"] = "rbxassetid://10734888864",
	["mic-2"] = "rbxassetid://10734888430",
	["mic-off"] = "rbxassetid://10734888646",
	["microscope"] = "rbxassetid://10734889106",
	["microwave"] = "rbxassetid://10734895076",
	["milestone"] = "rbxassetid://10734895310",
	["minimize"] = "rbxassetid://10734895698",
	["minimize-2"] = "rbxassetid://10734895530",
	["minus"] = "rbxassetid://10734896206",
	["minus-circle"] = "rbxassetid://10734895856",
	["minus-square"] = "rbxassetid://10734896029",
	["monitor"] = "rbxassetid://10734896881",
	["monitor-off"] = "rbxassetid://10734896360",
	["monitor-speaker"] = "rbxassetid://10734896512",
	["moon"] = "rbxassetid://10734897102",
	["more-horizontal"] = "rbxassetid://10734897250",
	["more-vertical"] = "rbxassetid://10734897387",
	["mountain"] = "rbxassetid://10734897956",
	["mountain-snow"] = "rbxassetid://10734897665",
	["mouse"] = "rbxassetid://10734898592",
	["mouse-pointer"] = "rbxassetid://10734898476",
	["mouse-pointer-2"] = "rbxassetid://10734898194",
	["mouse-pointer-click"] = "rbxassetid://10734898355",
	["move"] = "rbxassetid://10734900011",
	["move-3d"] = "rbxassetid://10734898756",
	["move-diagonal"] = "rbxassetid://10734899164",
	["move-diagonal-2"] = "rbxassetid://10734898934",
	["move-horizontal"] = "rbxassetid://10734899414",
	["move-vertical"] = "rbxassetid://10734899821",
	["music"] = "rbxassetid://10734905958",
	["music-2"] = "rbxassetid://10734900215",
	["music-3"] = "rbxassetid://10734905665",
	["music-4"] = "rbxassetid://10734905823",
	["navigation"] = "rbxassetid://10734906744",
	["navigation-2"] = "rbxassetid://10734906332",
	["navigation-2-off"] = "rbxassetid://10734906144",
	["navigation-off"] = "rbxassetid://10734906580",
	["network"] = "rbxassetid://10734906975",
	["newspaper"] = "rbxassetid://10734907168",
	["octagon"] = "rbxassetid://10734907361",
	["option"] = "rbxassetid://10734907649",
	["outdent"] = "rbxassetid://10734907933",
	["package"] = "rbxassetid://10734909540",
	["package-2"] = "rbxassetid://10734908151",
	["package-check"] = "rbxassetid://10734908384",
	["package-minus"] = "rbxassetid://10734908626",
	["package-open"] = "rbxassetid://10734908793",
	["package-plus"] = "rbxassetid://10734909016",
	["package-search"] = "rbxassetid://10734909196",
	["package-x"] = "rbxassetid://10734909375",
	["paint-bucket"] = "rbxassetid://10734909847",
	["paintbrush"] = "rbxassetid://10734910187",
	["paintbrush-2"] = "rbxassetid://10734910030",
	["palette"] = "rbxassetid://10734910430",
	["palmtree"] = "rbxassetid://10734910680",
	["paperclip"] = "rbxassetid://10734910927",
	["party-popper"] = "rbxassetid://10734918735",
	["pause"] = "rbxassetid://10734919336",
	["pause-circle"] = "rbxassetid://10735024209",
	["pause-octagon"] = "rbxassetid://10734919143",
	["pen-tool"] = "rbxassetid://10734919503",
	["pencil"] = "rbxassetid://10734919691",
	["percent"] = "rbxassetid://10734919919",
	["person-standing"] = "rbxassetid://10734920149",
	["phone"] = "rbxassetid://10734921524",
	["phone-call"] = "rbxassetid://10734920305",
	["phone-forwarded"] = "rbxassetid://10734920508",
	["phone-incoming"] = "rbxassetid://10734920694",
	["phone-missed"] = "rbxassetid://10734920845",
	["phone-off"] = "rbxassetid://10734921077",
	["phone-outgoing"] = "rbxassetid://10734921288",
	["pie-chart"] = "rbxassetid://10734921727",
	["piggy-bank"] = "rbxassetid://10734921935",
	["pin"] = "rbxassetid://10734922324",
	["pin-off"] = "rbxassetid://10734922180",
	["pipette"] = "rbxassetid://10734922497",
	["pizza"] = "rbxassetid://10734922774",
	["plane"] = "rbxassetid://10734922971",
	["play"] = "rbxassetid://10734923549",
	["play-circle"] = "rbxassetid://10734923214",
	["plus"] = "rbxassetid://10734924532",
	["plus-circle"] = "rbxassetid://10734923868",
	["plus-square"] = "rbxassetid://10734924219",
	["podcast"] = "rbxassetid://10734929553",
	["pointer"] = "rbxassetid://10734929723",
	["pound-sterling"] = "rbxassetid://10734929981",
	["power"] = "rbxassetid://10734930466",
	["power-off"] = "rbxassetid://10734930257",
	["printer"] = "rbxassetid://10734930632",
	["puzzle"] = "rbxassetid://10734930886",
	["quote"] = "rbxassetid://10734931234",
	["radio"] = "rbxassetid://10734931596",
	["radio-receiver"] = "rbxassetid://10734931402",
	["rectangle-horizontal"] = "rbxassetid://10734931777",
	["rectangle-vertical"] = "rbxassetid://10734932081",
	["recycle"] = "rbxassetid://10734932295",
	["redo"] = "rbxassetid://10734932822",
	["redo-2"] = "rbxassetid://10734932586",
	["refresh-ccw"] = "rbxassetid://10734933056",
	["refresh-cw"] = "rbxassetid://10734933222",
	["refrigerator"] = "rbxassetid://10734933465",
	["regex"] = "rbxassetid://10734933655",
	["repeat"] = "rbxassetid://10734933966",
	["repeat-1"] = "rbxassetid://10734933826",
	["reply"] = "rbxassetid://10734934252",
	["reply-all"] = "rbxassetid://10734934132",
	["rewind"] = "rbxassetid://10734934347",
	["rocket"] = "rbxassetid://10734934585",
	["rocking-chair"] = "rbxassetid://10734939942",
	["rotate-3d"] = "rbxassetid://10734940107",
	["rotate-ccw"] = "rbxassetid://10734940376",
	["rotate-cw"] = "rbxassetid://10734940654",
	["rss"] = "rbxassetid://10734940825",
	["ruler"] = "rbxassetid://10734941018",
	["russian-ruble"] = "rbxassetid://10734941199",
	["sailboat"] = "rbxassetid://10734941354",
	["save"] = "rbxassetid://10734941499",
	["scale"] = "rbxassetid://10734941912",
	["scale-3d"] = "rbxassetid://10734941739",
	["scaling"] = "rbxassetid://10734942072",
	["scan"] = "rbxassetid://10734942565",
	["scan-face"] = "rbxassetid://10734942198",
	["scan-line"] = "rbxassetid://10734942351",
	["scissors"] = "rbxassetid://10734942778",
	["screen-share"] = "rbxassetid://10734943193",
	["screen-share-off"] = "rbxassetid://10734942967",
	["scroll"] = "rbxassetid://10734943448",
	["search"] = "rbxassetid://10734943674",
	["send"] = "rbxassetid://10734943902",
	["separator-horizontal"] = "rbxassetid://10734944115",
	["separator-vertical"] = "rbxassetid://10734944326",
	["server"] = "rbxassetid://10734949856",
	["server-cog"] = "rbxassetid://10734944444",
	["server-crash"] = "rbxassetid://10734944554",
	["server-off"] = "rbxassetid://10734944668",
	["settings"] = "rbxassetid://10734950309",
	["settings-2"] = "rbxassetid://10734950020",
	["share"] = "rbxassetid://10734950813",
	["share-2"] = "rbxassetid://10734950553",
	["sheet"] = "rbxassetid://10734951038",
	["shield"] = "rbxassetid://10734951847",
	["shield-alert"] = "rbxassetid://10734951173",
	["shield-check"] = "rbxassetid://10734951367",
	["shield-close"] = "rbxassetid://10734951535",
	["shield-off"] = "rbxassetid://10734951684",
	["shirt"] = "rbxassetid://10734952036",
	["shopping-bag"] = "rbxassetid://10734952273",
	["shopping-cart"] = "rbxassetid://10734952479",
	["shovel"] = "rbxassetid://10734952773",
	["shower-head"] = "rbxassetid://10734952942",
	["shrink"] = "rbxassetid://10734953073",
	["shrub"] = "rbxassetid://10734953241",
	["shuffle"] = "rbxassetid://10734953451",
	["sidebar"] = "rbxassetid://10734954301",
	["sidebar-close"] = "rbxassetid://10734953715",
	["sidebar-open"] = "rbxassetid://10734954000",
	["sigma"] = "rbxassetid://10734954538",
	["signal"] = "rbxassetid://10734961133",
	["signal-high"] = "rbxassetid://10734954807",
	["signal-low"] = "rbxassetid://10734955080",
	["signal-medium"] = "rbxassetid://10734955336",
	["signal-zero"] = "rbxassetid://10734960878",
	["siren"] = "rbxassetid://10734961284",
	["skip-back"] = "rbxassetid://10734961526",
	["skip-forward"] = "rbxassetid://10734961809",
	["skull"] = "rbxassetid://10734962068",
	["slack"] = "rbxassetid://10734962339",
	["slash"] = "rbxassetid://10734962600",
	["slice"] = "rbxassetid://10734963024",
	["sliders"] = "rbxassetid://10734963400",
	["sliders-horizontal"] = "rbxassetid://10734963191",
	["smartphone"] = "rbxassetid://10734963940",
	["smartphone-charging"] = "rbxassetid://10734963671",
	["smile"] = "rbxassetid://10734964441",
	["smile-plus"] = "rbxassetid://10734964188",
	["snowflake"] = "rbxassetid://10734964600",
	["sofa"] = "rbxassetid://10734964852",
	["sort-asc"] = "rbxassetid://10734965115",
	["sort-desc"] = "rbxassetid://10734965287",
	["speaker"] = "rbxassetid://10734965419",
	["sprout"] = "rbxassetid://10734965572",
	["square"] = "rbxassetid://10734965702",
	["star"] = "rbxassetid://10734966248",
	["star-half"] = "rbxassetid://10734965897",
	["star-off"] = "rbxassetid://10734966097",
	["stethoscope"] = "rbxassetid://10734966384",
	["sticker"] = "rbxassetid://10734972234",
	["sticky-note"] = "rbxassetid://10734972463",
	["stop-circle"] = "rbxassetid://10734972621",
	["stretch-horizontal"] = "rbxassetid://10734972862",
	["stretch-vertical"] = "rbxassetid://10734973130",
	["strikethrough"] = "rbxassetid://10734973290",
	["subscript"] = "rbxassetid://10734973457",
	["sun"] = "rbxassetid://10734974297",
	["sun-dim"] = "rbxassetid://10734973645",
	["sun-medium"] = "rbxassetid://10734973778",
	["sun-moon"] = "rbxassetid://10734973999",
	["sun-snow"] = "rbxassetid://10734974130",
	["sunrise"] = "rbxassetid://10734974522",
	["sunset"] = "rbxassetid://10734974689",
	["superscript"] = "rbxassetid://10734974850",
	["swiss-franc"] = "rbxassetid://10734975024",
	["switch-camera"] = "rbxassetid://10734975214",
	["sword"] = "rbxassetid://10734975486",
	["swords"] = "rbxassetid://10734975692",
	["syringe"] = "rbxassetid://10734975932",
	["table"] = "rbxassetid://10734976230",
	["table-2"] = "rbxassetid://10734976097",
	["tablet"] = "rbxassetid://10734976394",
	["tag"] = "rbxassetid://10734976528",
	["tags"] = "rbxassetid://10734976739",
	["target"] = "rbxassetid://10734977012",
	["tent"] = "rbxassetid://10734981750",
	["terminal"] = "rbxassetid://10734982144",
	["terminal-square"] = "rbxassetid://10734981995",
	["text-cursor"] = "rbxassetid://10734982395",
	["text-cursor-input"] = "rbxassetid://10734982297",
	["thermometer"] = "rbxassetid://10734983134",
	["thermometer-snowflake"] = "rbxassetid://10734982571",
	["thermometer-sun"] = "rbxassetid://10734982771",
	["thumbs-down"] = "rbxassetid://10734983359",
	["thumbs-up"] = "rbxassetid://10734983629",
	["ticket"] = "rbxassetid://10734983868",
	["timer"] = "rbxassetid://10734984606",
	["timer-off"] = "rbxassetid://10734984138",
	["timer-reset"] = "rbxassetid://10734984355",
	["toggle-left"] = "rbxassetid://10734984834",
	["toggle-right"] = "rbxassetid://10734985040",
	["tornado"] = "rbxassetid://10734985247",
	["toy-brick"] = "rbxassetid://10747361919",
	["train"] = "rbxassetid://10747362105",
	["trash"] = "rbxassetid://10747362393",
	["trash-2"] = "rbxassetid://10747362241",
	["tree-deciduous"] = "rbxassetid://10747362534",
	["tree-pine"] = "rbxassetid://10747362748",
	["trees"] = "rbxassetid://10747363016",
	["trending-down"] = "rbxassetid://10747363205",
	["trending-up"] = "rbxassetid://10747363465",
	["triangle"] = "rbxassetid://10747363621",
	["trophy"] = "rbxassetid://10747363809",
	["truck"] = "rbxassetid://10747364031",
	["tv"] = "rbxassetid://10747364593",
	["tv-2"] = "rbxassetid://10747364302",
	["type"] = "rbxassetid://10747364761",
	["umbrella"] = "rbxassetid://10747364971",
	["underline"] = "rbxassetid://10747365191",
	["undo"] = "rbxassetid://10747365484",
	["undo-2"] = "rbxassetid://10747365359",
	["unlink"] = "rbxassetid://10747365771",
	["unlink-2"] = "rbxassetid://10747397871",
	["unlock"] = "rbxassetid://10747366027",
	["upload"] = "rbxassetid://10747366434",
	["upload-cloud"] = "rbxassetid://10747366266",
	["usb"] = "rbxassetid://10747366606",
	["user"] = "rbxassetid://10747373176",
	["user-check"] = "rbxassetid://10747371901",
	["user-cog"] = "rbxassetid://10747372167",
	["user-minus"] = "rbxassetid://10747372346",
	["user-plus"] = "rbxassetid://10747372702",
	["user-x"] = "rbxassetid://10747372992",
	["users"] = "rbxassetid://10747373426",
	["utensils"] = "rbxassetid://10747373821",
	["utensils-crossed"] = "rbxassetid://10747373629",
	["venetian-mask"] = "rbxassetid://10747374003",
	["verified"] = "rbxassetid://10747374131",
	["vibrate"] = "rbxassetid://10747374489",
	["vibrate-off"] = "rbxassetid://10747374269",
	["video"] = "rbxassetid://10747374938",
	["video-off"] = "rbxassetid://10747374721",
	["view"] = "rbxassetid://10747375132",
	["voicemail"] = "rbxassetid://10747375281",
	["volume"] = "rbxassetid://10747376008",
	["volume-1"] = "rbxassetid://10747375450",
	["volume-2"] = "rbxassetid://10747375679",
	["volume-x"] = "rbxassetid://10747375880",
	["wallet"] = "rbxassetid://10747376205",
	["wand"] = "rbxassetid://10747376565",
	["wand-2"] = "rbxassetid://10747376349",
	["watch"] = "rbxassetid://10747376722",
	["waves"] = "rbxassetid://10747376931",
	["webcam"] = "rbxassetid://10747381992",
	["wifi"] = "rbxassetid://10747382504",
	["wifi-off"] = "rbxassetid://10747382268",
	["wind"] = "rbxassetid://10747382750",
	["wrap-text"] = "rbxassetid://10747383065",
	["wrench"] = "rbxassetid://10747383470",
	["x"] = "rbxassetid://10747384394",
	["x-circle"] = "rbxassetid://10747383819",
	["x-octagon"] = "rbxassetid://10747384037",
	["x-square"] = "rbxassetid://10747384217",
	["zoom-in"] = "rbxassetid://10747384552",
	["zoom-out"] = "rbxassetid://10747384679",
}
return assets

end)() end,
    [60] = function()local wax,script,require=ImportGlobals(60)local ImportGlobals return (function(...)local cF = require(script.Parent.Parent.utilities.customFunctions)
local cloneref = cF.cloneref

local services = {}

services.UserInputService = cloneref(game:GetService("UserInputService"))
services.RunService = cloneref(game:GetService("RunService"))
services.TextService = cloneref(game:GetService("TextService"))
services.ContextActionService = cloneref(game:GetService("ContextActionService"))

return services
end)() end,
    [61] = function()local wax,script,require=ImportGlobals(61)local ImportGlobals return (function(...)local themeSystem = {}
local fusion = require(script.Parent.Fusion)
local default = require(script.default)
local value = fusion.Value
local observe = fusion.Observer

local get = require(script.Parent.Parent.utilities.get)
local animate = require(script.Parent.Parent.utilities.animate)
local functionsOnChange = {}

local currentTheme = value({})
observe(currentTheme):onChange(function()
    for _,func in ipairs(functionsOnChange) do
        task.spawn(func)
    end
end)

local palletteCheck = {
    text = "Color3",
    text2 = "Color3",
    background = "Color3",
    primary = "Color3",
    secondary = "Color3",
    accent = "Color3",
    red = "Color3",
    green = "Color3",
    selectedTab = "Color3",
    toggled = "Color3"
}

function themeSystem.create(pallette)
    for i,v in palletteCheck do
        if pallette[i] == nil then
            error('Missing value "'..tostring(i)..'"')
        end
        if typeof(pallette[i]) ~= "Color3" then
            error(i.." isn't type Color3")
        end
    end
    for i,_ in pallette do
        if palletteCheck[i] == nil then
            warn('Incorrect value removed "'..tostring(i)..'"')
            pallette[i] = nil
        end
    end
    currentTheme:set(pallette)
end

function themeSystem.get(name:string, animation:boolean)
    local theme = get(currentTheme)
    if theme[name] then
        if animation then
            return animate(function()
                return get(currentTheme)[name]
            end,40,1)
        else
            return theme[name]
        end
    else
        error(name.." isn't a theme element")
    end
end
currentTheme:set(default)
function themeSystem.onChange(func)
    table.insert(functionsOnChange, func)
end

return themeSystem
end)() end,
    [62] = function()local wax,script,require=ImportGlobals(62)local ImportGlobals return (function(...)return {
    text = Color3.fromHex("#e9e9ff"),
    text2 = Color3.fromHex("#d8d9ed"),
    background = Color3.fromHex("#040406"),
    primary = Color3.fromHex("#a3a4c2"),
    secondary = Color3.fromHex("#3d3d61"),
    accent = Color3.fromHex("#7273ac"),

    selectedTab = Color3.fromHex("#50579B"),
    toggled = Color3.fromRGB(118, 127, 211),

    red = Color3.fromRGB(255,105,97),
    green = Color3.fromRGB(96, 255, 136)
}
end)() end,
    [64] = function()local wax,script,require=ImportGlobals(64)local ImportGlobals return (function(...)local project = script.Parent.Parent

local data = require(project.Bundles.data)
local services = require(project.Bundles.services)
local ContextActionService = services.ContextActionService
local icons = require(project.Bundles.icons)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed
local observe = Fusion.Observer

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local animate = require(project.utilities.animate)
local customFont = require(project.utilities.customFont)

local UIPadding = require(project.simpleElements.UIPadding)

return function(buttonProperty)
	assert(buttonProperty.Name, ":Button missing property Name")
	assert(typeof(buttonProperty.Name) == "string", ("Name accepts type string got %s"):format(typeof(buttonProperty.Name)))
	assert(buttonProperty.Callback, ":Button missing property Callback")
	assert(typeof(buttonProperty.Callback) == "function", ("Callback accepts type function got %s"):format(typeof(buttonProperty.Callback)))
	if buttonProperty.Description then
		assert(typeof(buttonProperty.Description) == "string", ("Description accepts type string got %s"):format(typeof(buttonProperty.Description)))
	end
	if buttonProperty.IsEnabled then
		assert(typeof(buttonProperty.IsEnabled) == "boolean", ("IsEnabled accepts type boolean got %s"):format(typeof(buttonProperty.IsEnabled)))
	end
	if buttonProperty.HasPreview then
		assert(typeof(buttonProperty.HasPreview) == "boolean", ("HasPreview accepts type boolean got %s"):format(typeof(buttonProperty.HasPreview)))
	end
	if buttonProperty.PreviewAssetId then
		assert(typeof(buttonProperty.PreviewAssetId) == "string", ("PreviewAssetId accepts type string got %s"):format(typeof(buttonProperty.PreviewAssetId)))
	end
	if buttonProperty.ImageSize then
		assert(typeof(buttonProperty.ImageSize) == "UDim2", ("ImageSize accepts type UDim2 got %s"):format(typeof(buttonProperty.ImageSize)))
	end

	local name = value(buttonProperty.Name)
	local callback = value(buttonProperty.Callback)
	local isEnabled = value(buttonProperty.IsEnabled)
	local disabledText = value(buttonProperty.DisabledText or "Component Disabled")
	
	local previewAssetId = value(buttonProperty.PreviewAssetId)
	local imageSize = value(buttonProperty.ImageSize)

	local currentIndex = data.find(buttonProperty.uid)
	local selectedTab = data.find("selectedTab")
	local hoveredComponent = data.find("hoveredComponent")
	local isHidden = data.find("isHidden")
	local keys = data.find("keys")
	local buttonUID = data.add({}).uid

	local isHovered = value(false)
	observe(currentIndex.index):onChange(function()
		local index = get(currentIndex.index)
		if index == buttonProperty.index then
			isHovered:set(true)
			hoveredComponent:update("data", buttonProperty)
		else
			isHovered:set(false)
		end
	end)

	local buttonComponent = new "Frame" {
		Parent = buttonProperty.Parent,
		Size = UDim2.new(1,0,0,28),
		BackgroundColor3 = theme.get('selectedTab', true),
		BackgroundTransparency = animate(function()
			if get(isHovered) then
				return 0.55
			end
			return 1
		end,40,1),

		[children] = {
			new "ImageLabel" { -- Preview
				Visible = computed(function()
					if buttonProperty.HasPreview then
						return true
					end
					return false
				end),
				BackgroundColor3 = theme.get("background", true),
				Image = computed(function()
					return get(previewAssetId)
				end),
				BackgroundTransparency = animate(function()
					if get(isHovered) then
						return 0.65
					end
					return 1
				end,40,1),
				ImageTransparency = animate(function()
					if get(isHovered) then
						return 0
					end
					return 1
				end,40,1),
				Position = UDim2.new(1,5,0,0),
				Size = computed(function()
					return get(imageSize)
				end)
			},
			new "Frame" { -- Disabled Frame
				ZIndex = 2,
				Visible = computed(function()
					if get(isEnabled) ~= nil and not get(isEnabled)  then
						return true
					end
					return false
				end),
				BackgroundTransparency = 0.4,
				BackgroundColor3 = theme.get("background", true),
				Size = UDim2.fromScale(1,1),

				[children] = {
					new "ImageLabel" { -- Icon
						Size = UDim2.fromOffset(24,24),
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,5,0.5,0),
						BackgroundTransparency = 1,
						Image = icons.lock,
						ImageColor3 = theme.get("text", true)
					},
					new "TextLabel" {
						FontFace = customFont("Inter", Enum.FontWeight.Bold),
						BackgroundTransparency = 1,
						Size = UDim2.new(1,-24,0,14),
						TextColor3 = theme.get("text", true),
						TextScaled = true,
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,29,0.5,0),
						Text = computed(function()
							return get(disabledText)
						end),
						TextXAlignment = Enum.TextXAlignment.Left,
						RichText = true,

						[children] = {
							UIPadding(0,5)
						}
					}
				}
			},
			new "TextLabel" { -- Title
				FontFace = customFont("Inter", Enum.FontWeight.Medium),
				Size = UDim2.new(1,-39,0,14),
				BackgroundTransparency = 1,
				TextColor3 = theme.get("text", true),
				TextScaled = true,
				AnchorPoint = Vector2.new(0,0.5),
				Position = UDim2.new(0,0,0.5,0),
				Text = computed(function()
					return get(name)
				end),
				TextXAlignment = Enum.TextXAlignment.Left,
				RichText = true,

				[children] = {
					UIPadding(0,0,0,5)
				}
			},
			new "ImageLabel" { -- Icon
				Size = UDim2.fromOffset(24,24),
				AnchorPoint = Vector2.new(1,0.5),
				Position = UDim2.new(1,-5,0.5,0),
				BackgroundTransparency = 1,
				Image = icons.click,
				ImageColor3 = theme.get("text", true)
			}
		}
	}

	ContextActionService:BindActionAtPriority(buttonUID, function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= buttonProperty.uid or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		task.spawn(get(callback))
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).enter)
	if get(currentIndex.index) == buttonProperty.index then
		isHovered:set(true)
	end

	local returnFunctions = {}
	function returnFunctions:SetName(newText)
		assert(typeof(newText) == "string", (":SetName accepts type string got %s"):format(typeof(newText)))
		name:set(newText)
	end
	function returnFunctions:SetCallback(newFunction)
		assert(typeof(newFunction) == "function", (":SetCallback accepts type function got %s"):format(typeof(newFunction)))
		callback:set(newFunction)
	end
	function returnFunctions:Fire()
		task.spawn(get(callback))
	end
	function returnFunctions:SetPreviewAssetId(newPreviewAssetId)
		previewAssetId:set(newPreviewAssetId)
	end
	function returnFunctions:SetImageSize(newImageSize)
		imageSize:set(newImageSize)
	end
	return returnFunctions
end
end)() end,
    [65] = function()local wax,script,require=ImportGlobals(65)local ImportGlobals return (function(...)local project = script.Parent.Parent

local data = require(project.Bundles.data)
local theme = require(project.Bundles.themeSystem)
local services = require(project.Bundles.services)
local ContextActionService = services.ContextActionService
local icons = require(project.Bundles.icons)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed
local observe = Fusion.Observer

local get = require(project.utilities.get)
local animate = require(project.utilities.animate)
local customFont = require(project.utilities.customFont)
local references = require(project.utilities.references)

local UIListLayout = require(project.simpleElements.UIListLayout)
local UIPadding = require(project.simpleElements.UIPadding)
local SO = Enum.SortOrder
local FD = Enum.FillDirection
local HA = Enum.HorizontalAlignment
local VA = Enum.VerticalAlignment

return function(dropdownProperty)
	assert(dropdownProperty.Name, ":Dropdown missing property Name")
	assert(typeof(dropdownProperty.Name) == "string", ("Name accepts type string got %s"):format(typeof(dropdownProperty.Name)))
	assert(dropdownProperty.Callback, ":Dropdown missing property Callback")
	assert(typeof(dropdownProperty.Callback) == "function", ("Callback accepts type function got %s"):format(typeof(dropdownProperty.Callback)))
	if dropdownProperty.Multiselect then
		assert(typeof(dropdownProperty.Multiselect) == "boolean", ("Multiselect accepts type boolean got %s"):format(typeof(dropdownProperty.Multiselect)))
	end
	if dropdownProperty.Description then
		assert(typeof(dropdownProperty.Description) == "string", ("Description accepts type string got %s"):format(typeof(dropdownProperty.Description)))
	end
	if dropdownProperty.IsEnabled then
		assert(typeof(dropdownProperty.IsEnabled) == "boolean", ("IsEnabled accepts type boolean got %s"):format(typeof(dropdownProperty.IsEnabled)))
	end
	if dropdownProperty.HasPreview then
		assert(typeof(dropdownProperty.HasPreview) == "boolean", ("HasPreview accepts type boolean got %s"):format(typeof(dropdownProperty.HasPreview)))
	end
	if dropdownProperty.PreviewAssetId then
		assert(typeof(dropdownProperty.PreviewAssetId) == "string", ("PreviewAssetId accepts type string got %s"):format(typeof(dropdownProperty.PreviewAssetId)))
	end
	if dropdownProperty.ImageSize then
		assert(typeof(dropdownProperty.ImageSize) == "UDim2", ("ImageSize accepts type UDim2 got %s"):format(typeof(dropdownProperty.ImageSize)))
	end

	local name = value(dropdownProperty.Name)
	local items = value({})
	local callback = value(dropdownProperty.Callback)
	local isEnabled = value(dropdownProperty.IsEnabled)
	local disabledText = value(dropdownProperty.DisabledText or "Component Disabled")

	local previewAssetId = value(dropdownProperty.PreviewAssetId)
	local imageSize = value(dropdownProperty.ImageSize)

	local currentTab = data.find(dropdownProperty.uid)
	local selectedTab = data.find("selectedTab")
	local hoveredComponent = data.find("hoveredComponent")
	local isInDropdown = data.find("isInDropdown")
	local isHidden = data.find("isHidden")
	local keys = data.find("keys")
	local currentDropdown = data.add({data = dropdownProperty})
	local dropdownIndex = data.add({index = 1})
	local totalDropdownIndex = data.add({total = 0})
	local dropdownUID = data.add({}).uid

	local dropdownLists = references.get("tabWindowList")

	local selectedItems = value({})
	local itemElements = value({})
	local isHovered = value(false)
	observe(currentTab.index):onChange(function()
		local index = get(currentTab.index)
		if index == dropdownProperty.index then
			isHovered:set(true)
			hoveredComponent:update("data", dropdownProperty)
		else
			isHovered:set(false)
		end
	end)

	local dropdownComponent = new "Frame" {
		Parent = dropdownProperty.Parent,
		Size = UDim2.new(1,0,0,28),
		BackgroundColor3 = theme.get('selectedTab', true),
		BackgroundTransparency = animate(function()
			if get(isHovered) then
				return 0.55
			end
			return 1
		end,40,1),

		[children] = {
			new "ImageLabel" { -- Preview
				Visible = computed(function()
					if dropdownProperty.HasPreview then
						return true
					end
					return false
				end),
				BackgroundColor3 = theme.get("background", true),
				Image = computed(function()
					return get(previewAssetId)
				end),
				BackgroundTransparency = animate(function()
					if get(isHovered) then
						return 0.65
					end
					return 1
				end,40,1),
				ImageTransparency = animate(function()
					if get(isHovered) then
						return 0
					end
					return 1
				end,40,1),
				Position = UDim2.new(1,5,0,0),
				Size = computed(function()
					return get(imageSize)
				end)
			},
			new "Frame" { -- Disabled Frame
				ZIndex = 2,
				Visible = computed(function()
					if get(isEnabled) ~= nil and not get(isEnabled) then
						return true
					end
					return false
				end),
				BackgroundTransparency = 0.4,
				BackgroundColor3 = theme.get("background", true),
				Size = UDim2.fromScale(1,1),

				[children] = {
					new "ImageLabel" { -- Icon
						Size = UDim2.fromOffset(24,24),
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,5,0.5,0),
						BackgroundTransparency = 1,
						Image = icons.lock,
						ImageColor3 = theme.get("text", true)
					},
					new "TextLabel" {
						FontFace = customFont("Inter", Enum.FontWeight.Bold),
						BackgroundTransparency = 1,
						Size = UDim2.new(1,-24,0,14),
						TextColor3 = theme.get("text", true),
						TextScaled = true,
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,29,0.5,0),
						Text = computed(function()
							return get(disabledText)
						end),
						TextXAlignment = Enum.TextXAlignment.Left,
						RichText = true,

						[children] = {
							UIPadding(0,5)
						}
					}
				}
			},
			new "TextLabel" { -- Name
				FontFace = customFont("Inter", Enum.FontWeight.Medium),
				Size = UDim2.new(1,-39,0,14),
				BackgroundTransparency = 1,
				TextColor3 = theme.get("text", true),
				TextScaled = true,
				AnchorPoint = Vector2.new(0,0.5),
				Position = UDim2.new(0,0,0.5,0),
				Text = computed(function()
					return get(name)
				end),
				TextXAlignment = Enum.TextXAlignment.Left,
				RichText = true,

				[children] = {
					UIPadding(0,0,0,5)
				}
			},
			new "ImageLabel" { -- Icon
				Size = UDim2.fromOffset(24,24),
				AnchorPoint = Vector2.new(1,0.5),
				Position = UDim2.new(1,-5,0.5,0),
				BackgroundTransparency = 1,
				Image = computed(function()
					if dropdownProperty.Multiselect then
						return icons.multiDropdown
					end
					return icons.normalDropdown
				end),
				ImageColor3 = theme.get("text", true)
			}
		}
	}
	local listComponent = new "Frame" {
		Visible = computed(function()
			if get(selectedTab.data).uid == currentDropdown.uid then
				return true
			end
			return false
		end),
		Parent = dropdownLists,
		BackgroundTransparency = 1,
		Size = UDim2.new(1,0,0,28),
		AutomaticSize = Enum.AutomaticSize.Y,

		[children] = {
			UIListLayout(SO.LayoutOrder,FD.Vertical,HA.Center,VA.Top),
			itemElements
		}
	}

	ContextActionService:BindActionAtPriority(dropdownUID.." Open", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= dropdownProperty.uid or get(isInDropdown.boolean) or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		selectedTab:update("data", currentDropdown)
		isInDropdown:update("boolean", true)
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).enter)

	ContextActionService:BindActionAtPriority(dropdownUID.." Close", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= currentDropdown.uid then
			return Enum.ContextActionResult.Pass
		end
		selectedTab:update("data", get(currentTab.data))
		task.delay(0.001, function()
			isInDropdown:update("boolean", false)
		end)
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).back)

	ContextActionService:BindActionAtPriority(dropdownUID.." Up", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= currentDropdown.uid or not get(isInDropdown.boolean) then
			return Enum.ContextActionResult.Pass
		end
		local current = get(dropdownIndex.index)
		if get(totalDropdownIndex.total) > 0 then
			dropdownIndex:update("index", math.clamp(current - 1, 1, get(totalDropdownIndex.total)))
		end
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).up)

	ContextActionService:BindActionAtPriority(dropdownUID.." Down", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= currentDropdown.uid or not get(isInDropdown.boolean) then
			return Enum.ContextActionResult.Pass
		end
		local current = get(dropdownIndex.index)
		if get(totalDropdownIndex.total) > 0 then
			dropdownIndex:update("index", math.clamp(current + 1, 1, get(totalDropdownIndex.total)))
		end
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).down)

	observe(items):onChange(function()
		local tableToSet = {}
		for _,item in get(items) do
			local itemImage,itemValue,isTable,previewImage,previewSize,hasPreview
			if typeof(item) == "table" then
				isTable = value(true)
				itemValue = value(item.Value)
				itemImage = value(item.Image)

				previewImage = value(item.PreviewAssetId or "")
				previewSize = value(item.ImageSize or UDim2.fromOffset(24,24))
				hasPreview = value(item.HasPreview or false)
			else
				isTable = value(false)
				itemValue = value(item)
			end
			local itemHover = value(false)
			local itemSelected = value(false)
			local itemIndex = get(totalDropdownIndex.total) + 1
			totalDropdownIndex:update("total", itemIndex)
			local itemUID = data.add({}).uid

			observe(dropdownIndex.index):onChange(function()
				local index = get(dropdownIndex.index)
				if index == itemIndex then
					itemHover:set(true)
				else
					itemHover:set(false)
				end
			end)
			observe(selectedItems):onChange(function()
				if table.find(get(selectedItems), get(itemValue)) then
					itemSelected:set(true)
				end
			end)

			local itemComponent = new "Frame" {
				Size = UDim2.new(1,0,0,18),
				BackgroundColor3 = theme.get("selectedTab", true),
				BackgroundTransparency = animate(function()
					if get(itemHover) then
						return 0.6
					end
					return 1
				end,40,1),

				[children] = {
					new "ImageLabel" { -- Preview
						Visible = computed(function()
							if get(isTable) and get(hasPreview) then
								return true
							end
							return false
						end),
						BackgroundColor3 = theme.get("background", true),
						Image = computed(function()
							if get(isTable) then
								return get(previewImage)
							end
							return ""
						end),
						BackgroundTransparency = animate(function()
							if get(itemHover) then
								return 0.65
							end
							return 1
						end,40,1),
						ImageTransparency = animate(function()
							if get(itemHover) then
								return 0
							end
							return 1
						end,40,1),
						Position = UDim2.new(1,5,0,0),
						Size = computed(function()
							if get(isTable) then
								return get(previewSize)
							end
							return UDim2.fromOffset(0,0)
						end)
					},
					new "Frame" {
						Size = UDim2.fromScale(1,1),
						BackgroundTransparency = 1,

						[children] = {
							UIPadding(5),
							UIListLayout(SO.LayoutOrder, FD.Horizontal,HA.Left,VA.Center, UDim.new(0,5)),
							new "ImageLabel" { -- Image
								Visible = computed(function()
									if get(itemImage) and get(itemImage) ~= "" then
										return true
									end
									return false
								end),
								Size = UDim2.fromOffset(16,16),
								BackgroundTransparency = 1,
								Image = computed(function()
									return itemImage and get(itemImage) or ""
								end),
								ImageColor3 = animate(function()
									if dropdownProperty.Multiselect then
										if get(itemSelected) then
											return theme.get("green")
										end
										return theme.get("red")
									end
									return theme.get("text")
								end,40,1)
							},
							new "TextLabel" { -- Name
								Size = computed(function()
									return itemImage and UDim2.new(1,-25,0,14) or UDim2.new(1,0,0,14)
								end),
								TextColor3 = animate(function()
									if dropdownProperty.Multiselect then
										if get(itemSelected) then
											return theme.get("green")
										end
										return theme.get("red")
									end
									return theme.get("text")
								end,40,1),
								FontFace = customFont("Inter"),
								Text = computed(function()
									return get(itemValue)
								end),
								TextScaled = true,
								BackgroundTransparency = 1,
								TextXAlignment = Enum.TextXAlignment.Left,
								RichText = true
							}
						}
					}
				}
			}

			ContextActionService:BindActionAtPriority(itemUID.." Return", function(_, inputState)
				if inputState ~= Enum.UserInputState.Begin then
					return Enum.ContextActionResult.Pass
				end
				if get(isHidden.boolean) or not get(itemHover) or get(selectedTab.data).uid ~= currentDropdown.uid or not get(isInDropdown.boolean) then
					return Enum.ContextActionResult.Pass
				end
				if dropdownProperty.Multiselect then
					itemSelected:set(not get(itemSelected))
					local tbl = get(selectedItems)
					if get(itemSelected) then
						table.insert(tbl, get(itemValue))
						selectedItems:set(tbl)
					else
						table.remove(tbl, table.find(tbl, get(itemValue)))
						selectedItems:set(tbl)
					end
					task.spawn(get(callback), get(selectedItems))
				else
					task.spawn(get(callback), get(itemValue))
					selectedTab:update("data", get(currentTab.data))
					task.delay(0.0001, function()
						isInDropdown:update("boolean", false)
					end)
				end
				return Enum.ContextActionResult.Sink
			end,false, Enum.ContextActionPriority.High.Value, get(keys.key).enter)

			if get(dropdownIndex.index) == itemIndex then
				itemHover:set(true)
			end

			table.insert(tableToSet, itemComponent)
		end
		itemElements:set(tableToSet)
	end)
	items:set(dropdownProperty.Items)

	if get(currentTab.index) == dropdownProperty.index then
		isHovered:set(true)
	end

	local returnFunctions = {}
	function returnFunctions:SetItems(newTable) -- Sets a new table of items
		assert(typeof(newTable) == "table", (":SetItems accepts type table got %s"):format(typeof(newTable)))
		selectedItems:set({})
		totalDropdownIndex:update("total", 0)
		dropdownIndex:update("index", 1)
		items:set(newTable)
	end
	function returnFunctions:SelectItem(item) -- Sets item used when Multiselect is false
		assert(not dropdownProperty.Multiselect, ":SelectItem is only usable when Multiselect is false")
		assert(typeof(item) == "string", (":SelectItem accepts type string got %s"):format(typeof(item)))
		local hasItem = false
		for _,v in get(items) do
			local itemValue = v
			if typeof(v) == "table" then
				itemValue = v.Value
			end
			if itemValue == item then
				hasItem = true
				break
			end
		end
		assert(hasItem, ":SelectItem couldn't find "..item)
		task.spawn(get(callback), item)
	end
	function returnFunctions:SelectItems(newSelecteds) -- Sets items used when Multiselect is true
		assert(dropdownProperty.Multiselect, ":SelectItems is only usable when Multiselect is true")
		assert(typeof(newSelecteds) == "table", (":SelectItems accepts type table got %s"):format(typeof(newSelecteds)))
		for i,v in newSelecteds do
			if not table.find(get(items), v) then
				warn(":SelectItems couldn't find ", v, "in Items. Removing it.")
				newSelecteds[i] = nil
			end
		end
		selectedItems:set(newSelecteds)
		task.spawn(get(callback), get(selectedItems))
	end
	function returnFunctions:SetPreviewAssetId(newPreviewAssetId)
		previewAssetId:set(newPreviewAssetId)
	end
	function returnFunctions:SetImageSize(newImageSize)
		imageSize:set(newImageSize)
	end
	return returnFunctions
end
end)() end,
    [66] = function()local wax,script,require=ImportGlobals(66)local ImportGlobals return (function(...)local project = script.Parent.Parent

local services = require(project.Bundles.services)
local UserInputService = services.UserInputService
local ContextActionService = services.ContextActionService
local data = require(project.Bundles.data)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed
local observe = Fusion.Observer
local ref = Fusion.Ref

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local animate = require(project.utilities.animate)
local customFont = require(project.utilities.customFont)
local icons = require(project.Bundles.icons)
local connections = require(project.utilities.connections)

local UIPadding = require(project.simpleElements.UIPadding)
local UICorner = require(project.simpleElements.UICorner)

return function(keybindProperty)
	assert(keybindProperty.Name, ":Keybind missing property Name")
	assert(typeof(keybindProperty.Name) == "string", ("Name accepts type string got %s"):format(typeof(keybindProperty.Name)))
	assert(keybindProperty.Callback, ":Keybind missing property Callback")
	assert(typeof(keybindProperty.Callback) == "function", ("Callback accepts type function got %s"):format(typeof(keybindProperty.Callback)))
	assert(keybindProperty.Bind, ":Keybind missing property Bind")
	assert(typeof(keybindProperty.Bind) == "EnumItem", ("Bind accepts type EnumItem got %s"):format(typeof(keybindProperty.Bind)))
	if keybindProperty.Description then
		assert(typeof(keybindProperty.Description) == "string", ("Description accepts type string got %s"):format(typeof(keybindProperty.Description)))
	end
	if keybindProperty.IsEnabled then
		assert(typeof(keybindProperty.IsEnabled) == "boolean", ("IsEnabled accepts type boolean got %s"):format(typeof(keybindProperty.IsEnabled)))
	end
	if keybindProperty.HasPreview then
		assert(typeof(keybindProperty.HasPreview) == "boolean", ("HasPreview accepts type boolean got %s"):format(typeof(keybindProperty.HasPreview)))
	end
	if keybindProperty.PreviewAssetId then
		assert(typeof(keybindProperty.PreviewAssetId) == "string", ("PreviewAssetId accepts type string got %s"):format(typeof(keybindProperty.PreviewAssetId)))
	end
	if keybindProperty.ImageSize then
		assert(typeof(keybindProperty.ImageSize) == "UDim2", ("ImageSize accepts type UDim2 got %s"):format(typeof(keybindProperty.ImageSize)))
	end

	local name = value(keybindProperty.Name)
	local callback = value(keybindProperty.Callback)
	local isToggle = value(keybindProperty.IsToggle)
	local isEnabled = value(keybindProperty.IsEnabled)
	local disabledText = value(keybindProperty.DisabledText or "Component Disabled")

	local previewAssetId = value(keybindProperty.PreviewAssetId)
	local imageSize = value(keybindProperty.ImageSize)

	local currentIndex = data.find(keybindProperty.uid)
	local selectedTab = data.find("selectedTab")
	local hoveredComponent = data.find("hoveredComponent")
	local isHidden = data.find("isHidden")
	local keys = data.find("keys")
	local currentBinds = data.find("currentBinds")
	local blacklistedBinds = data.find("blacklistedBinds")
	local keybindUID = data.add({}).uid

	local focused = value(false)
	local waiting = value(false)
	local toggleValue = value(false)
	local isHovered = value(false)
	local textboxRef = value()
	local currentBind = value()
	observe(currentIndex.index):onChange(function()
		local index = get(currentIndex.index)
		if index == keybindProperty.index then
			isHovered:set(true)
			hoveredComponent:update("data", keybindProperty)
		else
			isHovered:set(false)
		end
	end)

	if not table.find(get(blacklistedBinds.binds), Enum.KeyCode.Unknown) then
		local binds = get(blacklistedBinds.binds)
		table.insert(binds, Enum.KeyCode.Unknown)
		blacklistedBinds:update("binds", binds)
	end
	if not table.find(get(blacklistedBinds.binds), get(keys.key).enter) then
		local binds = get(blacklistedBinds.binds)
		table.insert(binds, get(keys.key).enter)
		blacklistedBinds:update("binds", binds)
	end

	if not table.find(get(blacklistedBinds.binds), keybindProperty.Bind) and not table.find(keys, keybindProperty.Bind) and not table.find(get(currentBinds.binds), keybindProperty.Bind) then
		currentBind:set(keybindProperty.Bind)
		local binds = get(currentBinds.binds)
		table.insert(binds, keybindProperty.Bind)
		currentBinds:update("binds", binds)
	else
		error("Blacklisted Bind. "..tostring(keybindProperty.Bind))
	end

	local keybindComponent = new "Frame" {
		Parent = keybindProperty.Parent,
		Size = UDim2.new(1,0,0,28),
		BackgroundColor3 = theme.get('selectedTab', true),
		BackgroundTransparency = animate(function()
			if get(isHovered) then
				return 0.55
			end
			return 1
		end,40,1),

		[children] = {
			new "ImageLabel" { -- Preview
				Visible = computed(function()
					if keybindProperty.HasPreview then
						return true
					end
					return false
				end),
				BackgroundColor3 = theme.get("background", true),
				Image = computed(function()
					return get(previewAssetId) or ""
				end),
				BackgroundTransparency = animate(function()
					if get(isHovered) then
						return 0.65
					end
					return 1
				end,40,1),
				ImageTransparency = animate(function()
					if get(isHovered) then
						return 0
					end
					return 1
				end,40,1),
				Position = UDim2.new(1,5,0,0),
				Size = computed(function()
					return get(imageSize) or UDim2.fromOffset(0,0)
				end)
			},
			new "Frame" { -- Disabled Frame
				ZIndex = 2,
				Visible = computed(function()
					if get(isEnabled) ~= nil and not get(isEnabled)  then
						return true
					end
					return false
				end),
				BackgroundTransparency = 0.4,
				BackgroundColor3 = theme.get("background", true),
				Size = UDim2.fromScale(1,1),

				[children] = {
					new "ImageLabel" { -- Icon
						Size = UDim2.fromOffset(24,24),
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,5,0.5,0),
						BackgroundTransparency = 1,
						Image = icons.lock,
						ImageColor3 = theme.get("text", true)
					},
					new "TextLabel" {
						FontFace = customFont("Inter", Enum.FontWeight.Bold),
						BackgroundTransparency = 1,
						Size = UDim2.new(1,-24,0,14),
						TextColor3 = theme.get("text", true),
						TextScaled = true,
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,29,0.5,0),
						Text = computed(function()
							return get(disabledText)
						end),
						TextXAlignment = Enum.TextXAlignment.Left,
						RichText = true,

						[children] = {
							UIPadding(0,5)
						}
					}
				}
			},
			new "TextLabel" { -- Name
				FontFace = customFont("Inter", Enum.FontWeight.Medium),
				Size = UDim2.new(1,-39,0,14),
				BackgroundTransparency = 1,
				TextColor3 = theme.get("text", true),
				TextScaled = true,
				AnchorPoint = Vector2.new(0,0.5),
				Position = UDim2.new(0,0,0.5,0),
				Text = computed(function()
					return get(name)
				end),
				TextXAlignment = Enum.TextXAlignment.Left,
				RichText = true,

				[children] = {
					UIPadding(0,0,0,5)
				}
			},
			new "Frame" { -- Keybind
				AnchorPoint = Vector2.new(1,0.5),
				Position = UDim2.new(1,-5,0.5,0),
				Size = UDim2.new(0,34,0,18),
				BackgroundTransparency = animate(function()
					if get(isToggle) then
						if get(toggleValue) then
							return 0.2
						end
					end
					return 0.65
				end,40,1),
				BackgroundColor3 = animate(function()
					if get(isToggle) then
						if get(toggleValue) then
							return theme.get("toggled")
						end
					end
					return theme.get("background")
				end,40,1),

				[children] = {
					UICorner(4),
					new "TextBox" {
						Interactable = computed(function()
							return get(focused)
						end),
						Size = UDim2.new(1,0,0,14),
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.fromScale(0,0.5),
						BackgroundTransparency = 1,
						FontFace = customFont("Inter", Enum.FontWeight.Medium),
						PlaceholderColor3 = theme.get("text2", true),
						PlaceholderText = ". . .",
						TextXAlignment = Enum.TextXAlignment.Center,
						TextScaled = true,
						Text = computed(function()
							if get(focused) then
								return ""
							end
							return get(currentBind).Name or ""
						end),
						TextColor3 = animate(function()
							if get(isToggle) then
								if get(toggleValue) then
									return theme.get("text")
								end
							end
							return theme.get("toggled")
						end,40,1),

						[ref] = textboxRef,

						[children] = {
							UIPadding(0,5)
						}
					}
				}
			}
		}
	}
	
	local function functionForBind(_, inputState)
		if inputState ~= Enum.UserInputState.Begin or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		if get(isToggle) then
			toggleValue:set(not get(toggleValue))
			task.spawn(get(callback), get(toggleValue))
			return Enum.ContextActionResult.Sink
		end
		task.spawn(get(callback))
		return Enum.ContextActionResult.Sink
	end
	ContextActionService:BindActionAtPriority(keybindUID.." Callback", functionForBind, false, 1, get(currentBind))

	ContextActionService:BindActionAtPriority(keybindUID.." Focus", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= keybindProperty.uid or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		focused:set(true)
		local textbox = get(textboxRef)
		textbox:CaptureFocus()
		task.wait()
		waiting:set(true)
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).enter)

	local connection = UserInputService.InputBegan:Connect(function(input, gameProcessed)
		if get(isHidden.boolean) then
			return
		end
		if not input.KeyCode then
			return
		end
		if not get(isHovered) or get(selectedTab.data).uid ~= keybindProperty.uid then
			return
		end
		if get(waiting) then
			if not gameProcessed then
				return
			end
			local textbox = get(textboxRef)
			focused:set(false)
			waiting:set(false)
			task.wait()
			if not table.find(blacklistedBinds, input.KeyCode) and not table.find(get(keys.key), input.KeyCode) and not table.find(get(currentBinds.binds), input.KeyCode) then
				currentBind:set(input.KeyCode)
				local binds = get(currentBinds.binds)
				table.remove(binds, table.find(binds, input.KeyCode))
				table.insert(binds, input.KeyCode)
				currentBinds:update("binds", binds)
				ContextActionService:UnbindAction(keybindUID.." Callback")
				ContextActionService:BindActionAtPriority(keybindUID.." Callback", functionForBind, false, 1, get(currentBind))
			else
				warn("Blacklisted bind.", input.KeyCode)
			end
			textbox.Text = get(currentBind).Name or ""
		end
	end)
	connections.add(connection)

	if get(currentIndex.index) == keybindProperty.index then
		isHovered:set(true)
	end

	local returnFunctions = {}
	function returnFunctions:SetBind(newBind)
		if not table.find(blacklistedBinds, newBind) and not table.find(get(keys.key), newBind) and not table.find(get(currentBinds.binds), newBind) then
			currentBind:set(newBind)
			local binds = get(currentBinds.binds)
			table.remove(binds, table.find(binds, newBind))
			table.insert(binds, newBind)
			currentBinds:update("binds", binds)
			ContextActionService:UnbindAction(keybindUID.." Callback")
			ContextActionService:BindActionAtPriority(keybindUID.." Callback", functionForBind, false, 1, get(currentBind))
		else
			warn("Blacklisted bind.", newBind)
		end
	end
	function returnFunctions:SetPreviewAssetId(newPreviewAssetId)
		previewAssetId:set(newPreviewAssetId)
	end
	function returnFunctions:SetImageSize(newImageSize)
		imageSize:set(newImageSize)
	end
	return returnFunctions
end
end)() end,
    [67] = function()local wax,script,require=ImportGlobals(67)local ImportGlobals return (function(...)local project = script.Parent.Parent

local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local customFont = require(project.utilities.customFont)

return function(labelProperty)
	assert(labelProperty.Text, "Label needs literally a string")

	local parent = value(labelProperty.Parent)
	local textValue = value(labelProperty.Text)

	local labelComponent = new "Frame" {
		Size = UDim2.new(1,0,0,22),
		Parent = get(parent),
		BackgroundTransparency = 1,

		[children] = {
			new "TextLabel" {
				FontFace = customFont("Inter", Enum.FontWeight.Medium),
				RichText = true,
				TextScaled = true,
				BackgroundTransparency = 1,
				Size = UDim2.new(1,-20,0,14),
				Position = UDim2.new(0,5,0.5,0),
				AnchorPoint = Vector2.new(0,0.5),
				TextXAlignment = Enum.TextXAlignment.Left,

				TextColor3 = theme.get("text", true),
				Text = computed(function()
					return get(textValue)
				end),

				[children] = {
					new "UITextSizeConstraint" {
						MinTextSize = 1,
						MaxTextSize = 14
					}
				}
			}
		}
	}

	local returnFunctions = {}
	function returnFunctions:SetText(newText)
		if typeof(newText) == "string" then
			textValue:set(newText)
		else
			error("you didnt give "..get(textValue).." a string for SetText!")
		end
	end
	return returnFunctions
end
end)() end,
    [68] = function()local wax,script,require=ImportGlobals(68)local ImportGlobals return (function(...)local project = script.Parent.Parent

local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local onevent = Fusion.OnEvent
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local animate = require(project.utilities.animate)
local getStringBounds = require(project.utilities.getStringBounds)
local customFont = require(project.utilities.customFont)

local UICorner = require(project.simpleElements.UICorner)
local UIPadding = require(project.simpleElements.UIPadding)
local UIListLayout = require(project.simpleElements.UIListLayout)
local SO = Enum.SortOrder
local FD = Enum.FillDirection
local HA = Enum.HorizontalAlignment
local VA = Enum.VerticalAlignment

local function roundTo(decimal, number)
    local point = 1
    for i=1,decimal do
        point = point*10
    end
    local stringed = tostring(math.round(number*point)/point)
    local tstringed = string.split(stringed, ".")
    if #tstringed == 1 then
        local zeros = ""
        for i=1,decimal do
            zeros = zeros.."0"
        end
        stringed = stringed.."."..zeros
    end
    return stringed
end

return function(notificationProperty)
    assert(notificationProperty.Title, "Notify needs a Title")
	assert(typeof(notificationProperty.Title) == "string", ("Title accepts type string got %s"):format(typeof(notificationProperty.Title)))
    assert(notificationProperty.Body, "Notfiy needs a Body")
    assert(typeof(notificationProperty.Body) == "string", ("Body accepts type string got %s"):format(typeof(notificationProperty.Body)))
    assert(notificationProperty.Duration, "Notify needs a Duration")
    assert(typeof(notificationProperty.Duration) == "number", ("Duration accepts type number got %s"):format(typeof(notificationProperty.Duration)))
    if notificationProperty.Image then
        assert(typeof(notificationProperty.Image) == "string", ("Image accepts type string got %s"):format(typeof(notificationProperty.Image)))
    end
    
    local title = value(notificationProperty.Title)
    local body = value(notificationProperty.Body)
    local duration = value(notificationProperty.Duration)
    local image = value(notificationProperty.Image)

    local loaded = value(false)
    local resized = value(false)
    local tickText = value("")

    local bodyBounds = value(getStringBounds(get(body), {
        Size = 12,
        Font = customFont("Inter", Enum.FontWeight.Medium),
        Width = 230
    }))

    local hideNotification = value(false)

    local newNotification
    newNotification = new "TextButton" {
        Visible = computed(function()
            if not get(hideNotification) then
                return true
            end
            return false
        end),
        AnchorPoint = Vector2.new(0,0.5),
        BackgroundTransparency = 1,

        Parent = notificationProperty.Parent,
        Size = animate(function()
            if get(loaded) then
                return UDim2.fromOffset(230,get(bodyBounds).Y + 28 + 4 + 10)
            end
            return UDim2.fromOffset(230,0)
        end,30,1),

        [onevent "MouseButton1Click"] = function()
            resized:set(false)
            task.wait(0.4)
            loaded:set(false)
            task.delay(0.3, function()
                hideNotification:set(true)
            end)
            task.wait(2)
            newNotification:Destroy()
        end,

        [children] = {
            new "CanvasGroup" {
                AnchorPoint = Vector2.new(0,0.5),
                BackgroundTransparency = 0.65,

                BackgroundColor3 = animate(function()
                    return theme.get("background")
                end,40,1),
                Size = UDim2.fromScale(1,1),
                Position = animate(function()
                    if get(resized) then
                        return UDim2.fromScale(0,0.5)
                    end
                    return UDim2.fromScale(1.2,0.5)
                end,30,1),

                [children] = {
                    UICorner(4),
                    UIListLayout(SO.LayoutOrder,FD.Horizontal,HA.Center,VA.Top),
                    new "Frame" {
                        Size = UDim2.fromScale(0.96,1),
                        BackgroundTransparency = 1,

                        [children] = {
                            UIPadding(5),
                            UIListLayout(SO.LayoutOrder, FD.Vertical,HA.Center,VA.Top,UDim.new(0,2)),
                            new "Frame" { -- Title
                                Size = UDim2.new(1,0,0,18),
                                BackgroundTransparency = 1,

                                [children] = {
                                    UIListLayout(SO.LayoutOrder, FD.Horizontal,HA.Left,VA.Center,UDim.new(0,3)),
                                    new "ImageLabel" {
                                        Visible = computed(function()
                                            if get(image) then
                                                return true
                                            end
                                            return false
                                        end),
                                        Size = UDim2.fromOffset(16,16),
                                        BackgroundTransparency = 1,
                                        Image = computed(function()
                                            if get(image) then
                                                return get(image)
                                            end
                                            return ""
                                        end),
                                        ImageColor3 = theme.get("text", true)
                                    },
                                    new "TextLabel" {
                                        Size = computed(function()
                                            if get(image) then
                                                return UDim2.new(1,-27,0,18)
                                            end
                                            return UDim2.new(1,0,0,18)
                                        end),
                                        BackgroundTransparency = 1,
                                        FontFace = customFont("Inter", Enum.FontWeight.Medium),
                                        TextXAlignment = Enum.TextXAlignment.Left,
                                        TextSize = 18,
                
                                        TextColor3 = animate(function()
                                            return theme.get("text")
                                        end,40,1),
                                        Text = get(title)
                                    }
                                }
                            },
                            new "TextLabel" { -- Body
                                Size = UDim2.new(1,0,0,1),
                                AutomaticSize = Enum.AutomaticSize.Y,
                                Position = UDim2.fromOffset(8,25),
                                BackgroundTransparency = 1,
                                FontFace = customFont("Inter", Enum.FontWeight.Medium),
                                TextSize = 12,
                                TextXAlignment = Enum.TextXAlignment.Left,
                                TextYAlignment = Enum.TextYAlignment.Top,
                                TextTransparency = 0.2,
                                TextWrapped = true,
                
                                TextColor3 = animate(function()
                                    return theme.get("text")
                                end,40,1),
                                Text = get(body)
                            },
                            new "TextLabel" { -- Timer
                                Size = UDim2.new(1,0,0,10),
                                AnchorPoint = Vector2.new(1,1),
                                Position = UDim2.new(1,-5,1,-5),
                                FontFace = customFont("Inter", Enum.FontWeight.Medium),
                                Text = computed(function()
                                    return get(tickText)
                                end),
                                TextColor3 = animate(function()
                                    return theme.get("text")
                                end,40,1),
                                TextSize = 10,
                                BackgroundTransparency = 1,
                                TextXAlignment = Enum.TextXAlignment.Right
                            }
                        }
                    },
                    new "Frame" { -- Color
                        Size = UDim2.fromScale(0.04,1),
                        BackgroundTransparency = 0.2,
                        BackgroundColor3 = theme.get("toggled", true)
                    }
                }
            }
        }
    }
    task.delay(0.4,function()
        resized:set(true)
    end)
    task.spawn(function()
        repeat task.wait() until get(resized)
        local startTick = tick()
        local endTick
        repeat
            endTick = tick() - startTick
            tickText:set(roundTo(1,math.clamp(endTick,0,get(duration))))
            task.wait()
        until endTick > get(duration)
    end)
    task.spawn(function()
        loaded:set(true)
        repeat task.wait() until get(resized)
        task.wait(get(duration))
        resized:set(false)
        task.wait(0.4)
        loaded:set(false)
        task.delay(0.3, function()
            hideNotification:set(true)
        end)
        task.wait(2)
        newNotification:Destroy()
    end)
end
end)() end,
    [69] = function()local wax,script,require=ImportGlobals(69)local ImportGlobals return (function(...)local project = script.Parent.Parent

local services = require(project.Bundles.services)
local ContextActionService = services.ContextActionService
local icons = require(project.Bundles.icons)
local data = require(project.Bundles.data)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed
local observe = Fusion.Observer

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local animate = require(project.utilities.animate)

local UIPadding = require(project.simpleElements.UIPadding)
local customFont = require(project.utilities.customFont)
local UICorner = require(project.simpleElements.UICorner)
local UIListLayout = require(project.simpleElements.UIListLayout)
local SO = Enum.SortOrder
local FD = Enum.FillDirection
local HA = Enum.HorizontalAlignment
local VA = Enum.VerticalAlignment

return function(sliderProperty)
	assert(sliderProperty.Name, ":Slider needs a Name")
	assert(typeof(sliderProperty.Name) == "string", ("Name accepts type string got %s"):format(typeof(sliderProperty.Name)))
	assert(sliderProperty.Max, ":Slider needs a Max")
	assert(typeof(sliderProperty.Max) == "number", ("Max accepts type number got %s"):format(typeof(sliderProperty.Max)))
	assert(sliderProperty.Min, ":Slider needs a Min")
	assert(typeof(sliderProperty.Min) == "number", ("Min accepts type number got %s"):format(typeof(sliderProperty.Min)))
	assert(sliderProperty.Callback, ":Slider needs a Callback")
	assert(typeof(sliderProperty.Callback) == "function", ("Callback accepts type function got %s"):format(typeof(sliderProperty.Callback)))
	if sliderProperty.Increment then
		assert(typeof(sliderProperty.Increment) == "number", ("Increment accepts type number got %s"):format(typeof(sliderProperty.Increment)))
	end
	if sliderProperty.UseHold then
		assert(typeof(sliderProperty.UseHold) == "boolean", ("UseHold accepts type boolean got %s"):format(typeof(sliderProperty.UseHold)))
	end
	if sliderProperty.DisplayAsPercent then
		assert(typeof(sliderProperty.DisplayAsPercent) == "boolean", ("DisplayAsPercent accepts type boolean got %s"):format(typeof(sliderProperty.DisplayAsPercent)))
	end
	if sliderProperty.Description then
		assert(typeof(sliderProperty.Description) == "string", ("Description accepts type string got %s"):format(typeof(sliderProperty.Description)))
	end
	if sliderProperty.IsEnabled then
		assert(typeof(sliderProperty.IsEnabled) == "boolean", ("IsEnabled accepts type boolean got %s"):format(typeof(sliderProperty.IsEnabled)))
	end
	if sliderProperty.HasPreview then
		assert(typeof(sliderProperty.HasPreview) == "boolean", ("HasPreview accepts type boolean got %s"):format(typeof(sliderProperty.HasPreview)))
	end
	if sliderProperty.PreviewAssetId then
		assert(typeof(sliderProperty.PreviewAssetId) == "string", ("PreviewAssetId accepts type string got %s"):format(typeof(sliderProperty.PreviewAssetId)))
	end
	if sliderProperty.ImageSize then
		assert(typeof(sliderProperty.ImageSize) == "UDim2", ("ImageSize accepts type UDim2 got %s"):format(typeof(sliderProperty.ImageSize)))
	end

	local name = value(sliderProperty.Name)
	local callback = value(sliderProperty.Callback)
	local max = value(sliderProperty.Max)
	local min = value(sliderProperty.Min)
	local useHold = value(sliderProperty.UseHold or false)
	local increment = value(sliderProperty.Increment or 1)
	local displayAsPercent = value(sliderProperty.DisplayAsPercent or false)
	local roundTo = value(sliderProperty.DecimalPlace or 2)
	local isEnabled = value(sliderProperty.IsEnabled)
	local disabledText = value(sliderProperty.DisabledText or "Component Disabled")

	local previewAssetId = value(sliderProperty.PreviewAssetId)
	local imageSize = value(sliderProperty.ImageSize)

	local currentIndex = data.find(sliderProperty.uid)
	local selectedTab = data.find("selectedTab")
	local hoveredComponent = data.find("hoveredComponent")
	local isHidden = data.find("isHidden")
	local keys = data.find("keys")
	local sliderUID = data.add({}).uid

	local isHovered = value(false)
	observe(currentIndex.index):onChange(function()
		local index = get(currentIndex.index)
		if index == sliderProperty.index then
			isHovered:set(true)
			hoveredComponent:update("data", sliderProperty)
		else
			isHovered:set(false)
		end
	end)

	local function mapValue(Value, MinA, MaxA, MinB, MaxB)
		return (1 - ((Value - MinA) / (MaxA - MinA))) * MinB + ((Value - MinA) / (MaxA - MinA)) * MaxB
	end
	local function round(Value)
		if get(roundTo) == 0 then
			return math.floor(Value)
		end
		return tonumber(string.format('%.' .. get(roundTo) .. 'f', Value))
	end

	local percentage = value(0)
	local percentageOf = value(get(min))
	observe(percentageOf):onChange(function()
		percentage:set(mapValue(get(percentageOf), get(min), get(max), 0, 1))
		if (get(isEnabled) ~= nil and not get(isEnabled)) then
			return
		end
		task.spawn(get(callback), get(percentageOf))
	end)

	local sliderComponent = new "Frame" {
		Parent = sliderProperty.Parent,
		Size = UDim2.new(1,0,0,28),
		BackgroundColor3 = theme.get('selectedTab', true),
		BackgroundTransparency = animate(function()
			if get(isHovered) then
				return 0.55
			end
			return 1
		end,40,1),

		[children] = {
			new "ImageLabel" { -- Preview
				Visible = computed(function()
					if sliderProperty.HasPreview then
						return true
					end
					return false
				end),
				BackgroundColor3 = theme.get("background", true),
				Image = computed(function()
					return get(previewAssetId)
				end),
				BackgroundTransparency = animate(function()
					if get(isHovered) then
						return 0.65
					end
					return 1
				end,40,1),
				ImageTransparency = animate(function()
					if get(isHovered) then
						return 0
					end
					return 1
				end,40,1),
				Position = UDim2.new(1,5,0,0),
				Size = computed(function()
					return get(imageSize)
				end)
			},
			new "Frame" { -- Disabled Frame
				ZIndex = 2,
				Visible = computed(function()
					if get(isEnabled) ~= nil and not get(isEnabled)  then
						return true
					end
					return false
				end),
				BackgroundTransparency = 0.4,
				BackgroundColor3 = theme.get("background", true),
				Size = UDim2.fromScale(1,1),

				[children] = {
					new "ImageLabel" { -- Icon
						Size = UDim2.fromOffset(24,24),
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,5,0.5,0),
						BackgroundTransparency = 1,
						Image = icons.lock,
						ImageColor3 = theme.get("text", true)
					},
					new "TextLabel" {
						FontFace = customFont("Inter", Enum.FontWeight.Bold),
						BackgroundTransparency = 1,
						Size = UDim2.new(1,-24,0,14),
						TextColor3 = theme.get("text", true),
						TextScaled = true,
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,29,0.5,0),
						Text = computed(function()
							return get(disabledText)
						end),
						TextXAlignment = Enum.TextXAlignment.Left,
						RichText = true,

						[children] = {
							UIPadding(0,5)
						}
					}
				}
			},
			new "TextLabel" { -- Name
				FontFace = customFont("Inter", Enum.FontWeight.Medium),
				Size = UDim2.new(0.6,-5,0,14),
				BackgroundTransparency = 1,
				TextColor3 = theme.get("text", true),
				TextScaled = true,
				AnchorPoint = Vector2.new(0,0.5),
				Position = UDim2.new(0,0,0.5,0),
				Text = computed(function()
					return get(name)
				end),
				TextXAlignment = Enum.TextXAlignment.Left,
				RichText = true,

				[children] = {
					UIPadding(0,0,0,5)
				}
			},
			new "Frame" { -- Slider
				Size = UDim2.new(0.4,0,0,16),
				AnchorPoint = Vector2.new(1,0.5),
				Position = UDim2.new(1,0,0.5,0),
				BackgroundTransparency = 1,

				[children] = {
					UIListLayout(SO.LayoutOrder,FD.Horizontal,HA.Right,VA.Center, UDim.new(0,10)),
					UIPadding(0,5,0,5),
					new "Frame" {
						Size = UDim2.new(0.6,-3,0.5,0),
						BackgroundTransparency = 0.65,
						BackgroundColor3 = theme.get("background", true),

						[children] = {
							UICorner(10000),
							new "Frame" {
								BackgroundColor3 = theme.get("toggled", true),
								Size = animate(function()
									return UDim2.fromScale(get(percentage),1)
								end,40,1),

								[children] = {
									UICorner(10000),
									new "ImageLabel" {
										Image = icons.circle,
										BackgroundTransparency = 1,
										ImageColor3 = theme.get("text", true),
										Size = UDim2.fromOffset(16,16),
										Position = UDim2.fromScale(1,0.5),
										AnchorPoint = Vector2.new(0.5,0.5)
									}
								}
							}
						}
					},
					new "Frame" {
						Size = UDim2.new(0.35,-5,1,0),
						BackgroundTransparency = 0.65,
						BackgroundColor3 = theme.get("background", true),

						[children] = {
							UICorner(4),
							new "TextLabel" {
								FontFace = customFont("Inter", Enum.FontWeight.Medium),
								Size = UDim2.new(1,0,0,12),
								BackgroundTransparency = 1,
								TextColor3 = theme.get("text", true),
								TextScaled = true,
								AnchorPoint = Vector2.new(0,0.5),
								Position = UDim2.new(0,0,0.5,0),
								Text = computed(function()
									if get(displayAsPercent) then
										return tostring(round(get(percentage) * 100)).."%"
									end
									return get(percentageOf)
								end),
								TextXAlignment = Enum.TextXAlignment.Center,
								RichText = true,

								[children] = {
									UIPadding(0,5,0,5)
								}
							}
						}
					}
				}
			}
		}
	}

	local isHeld = value(false)
	local currentKey = value()

	ContextActionService:BindActionAtPriority(sliderUID.." Left", function(_, inputState)
		if inputState == Enum.UserInputState.End and get(useHold) and get(currentKey) == get(keys.key).left then
			isHeld:set(false)
		end
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= sliderProperty.uid or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		isHeld:set(true)
		currentKey:set(get(keys.key).left)
		if get(useHold) then
			local rampMax = 50
			local currentRamp = 10
			while get(isHeld) and get(currentKey) == get(keys.key).left do
				percentageOf:set(math.clamp(get(percentageOf) - get(increment), get(min), get(max)))
				task.wait(1/currentRamp)
				currentRamp = math.clamp(currentRamp + 1, 0, rampMax)
			end
		else
			percentageOf:set(math.clamp(get(percentageOf) - get(increment), get(min), get(max)))
		end
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).left)

	ContextActionService:BindActionAtPriority(sliderUID.." Right", function(_, inputState)
		if inputState == Enum.UserInputState.End and get(useHold) and get(currentKey) == get(keys.key).right then
			isHeld:set(false)
		end
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= sliderProperty.uid or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		isHeld:set(true)
		currentKey:set(get(keys.key).right)
		if get(useHold) then
			local rampMax = 50
			local currentRamp = 10

			while get(isHeld) and get(currentKey) == get(keys.key).right do
				percentageOf:set(math.clamp(get(percentageOf) + get(increment), get(min), get(max)))
				task.wait(1/currentRamp)
				currentRamp = math.clamp(currentRamp + 1, 0, rampMax)
			end
		else
			percentageOf:set(math.clamp(get(percentageOf) + get(increment), get(min), get(max)))
		end
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).right)

	if get(currentIndex.index) == sliderProperty.index then
		isHovered:set(true)
	end

	local returnFunctions = {}
	function returnFunctions:SetValue(newValue)
		assert(typeof(newValue) == "number", (":SetValue accepts type number got %s"):format(typeof(newValue)))
		percentageOf:set(math.clamp(newValue, get(min), get(max)))
	end
	function returnFunctions:SetPreviewAssetId(newPreviewAssetId)
		previewAssetId:set(newPreviewAssetId)
	end
	function returnFunctions:SetImageSize(newImageSize)
		imageSize:set(newImageSize)
	end
	return returnFunctions
end
end)() end,
    [70] = function()local wax,script,require=ImportGlobals(70)local ImportGlobals return (function(...)local project = script.Parent.Parent

local data = require(project.Bundles.data)
local services = require(project.Bundles.services)
local ContextActionService = services.ContextActionService
local icons = require(project.Bundles.icons)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local observe = Fusion.Observer

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local animate = require(project.utilities.animate)
local customFont = require(project.utilities.customFont)

local UIListLayout = require(project.simpleElements.UIListLayout)
local UIPadding = require(project.simpleElements.UIPadding)
local SO = Enum.SortOrder
local FD = Enum.FillDirection
local HA = Enum.HorizontalAlignment
local VA = Enum.VerticalAlignment

return function(tabProperty)
	assert(tabProperty.Name, ":Tab missing property Name")
	assert(typeof(tabProperty.Image) == "string", ("Name accepts type string got %s"):format(typeof(tabProperty.Name)))
	if tabProperty.Image then
		assert(typeof(tabProperty.Image) == "string", ("Image accepts type string got %s"):format(typeof(tabProperty.Image)))
	end

	local selectedTab = data.find("selectedTab")
	local absoluteOffset = data.find("absoluteOffset")
	local currentIndex = data.find("currentIndex")
	local isInTab = data.find("isInTab")
	local isInDropdown = data.find("isInDropdown")
	local isHidden = data.find("isHidden")
	local keys = data.find("keys")

	-- Toggles

	local isHovered = value(false)
	if get(currentIndex.index) == tabProperty.index then
		isHovered:set(true)
	end
	observe(currentIndex.index):onChange(function()
		local index = get(currentIndex.index)
		if index == tabProperty.index then
			isHovered:set(true)
		else
			isHovered:set(false)
		end
	end)

	-- Positioning

	absoluteOffset = absoluteOffset.amount

	local tabComponent = new "Frame" {
		Name = tabProperty.uid,
		Parent = tabProperty.Parent,
		Size = UDim2.new(1,0,0,28),
		BackgroundColor3 = theme.get('selectedTab', true),
		BackgroundTransparency = animate(function()
			if get(isHovered) then
				return 0.55
			end
			return 1
		end,40,1),

		[children] = {
			UIPadding(0,0,0,5),
			new "Frame" {
				Size = UDim2.fromScale(1,1),
				BackgroundTransparency = 1,

				[children] = {
					UIListLayout(SO.LayoutOrder,FD.Horizontal,HA.Left,VA.Center,UDim.new(0,5)),
					new "TextLabel" {
						FontFace = customFont("Inter", Enum.FontWeight.Medium),
						AutomaticSize = Enum.AutomaticSize.X,
						Text = tabProperty.Name,
						TextColor3 = theme.get("text", true),
						Size = UDim2.new(0,1,0,14),
						TextScaled = true,
						BackgroundTransparency = 1,
						TextXAlignment = Enum.TextXAlignment.Left,
						RichText = true,

						[children] = {
							new "UISizeConstraint" {
								MaxSize = Vector2.new(tabProperty.SizeXInOffset-5-24-5-5-24,9e9),
								MinSize = Vector2.new(0,0)
							},
							new "UITextSizeConstraint" {
								MaxTextSize = 14,
								MinTextSize = 1
							}
						}
					},
					new "ImageLabel" {
						AnchorPoint = Vector2.new(0,0.5),
						BackgroundTransparency = 1,
						Position = UDim2.fromScale(0,0.5),

						ImageColor3 = theme.get("text", true),
						Image = tabProperty.Image,
						Size = UDim2.fromOffset(20,20)
					}
				}
			},
			new "ImageLabel" {
				Size = UDim2.fromOffset(24,24),
				AnchorPoint = Vector2.new(1,0.5),
				Position = UDim2.fromScale(1,0.5),
				BackgroundTransparency = 1,
				Image = icons.chevronRight,
				ImageColor3 = theme.get("text", true)
			}
		}
	}

	ContextActionService:BindActionAtPriority(tabProperty.uid.."OPEN", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or get(isInDropdown.boolean) or not get(isHovered) or get(selectedTab.data).uid == tabProperty.uid or get(isInTab.boolean) then
			return Enum.ContextActionResult.Pass
		end
		selectedTab:update("data", tabProperty)
		isInTab:update("boolean", true)
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).enter)

	ContextActionService:BindActionAtPriority(tabProperty.uid.."CLOSE", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or get(isInDropdown.boolean) or get(selectedTab.data).uid ~= tabProperty.uid then
			return Enum.ContextActionResult.Pass
		end
		selectedTab:update("data", {})
		isInTab:update("boolean", false)
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).back)
end
end)() end,
    [71] = function()local wax,script,require=ImportGlobals(71)local ImportGlobals return (function(...)local project = script.Parent.Parent

local data = require(project.Bundles.data)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local observe = Fusion.Observer
local value = Fusion.Value
local computed = Fusion.Computed

local theme = require(project.Bundles.themeSystem)
local get = require(project.utilities.get)
local animate = require(project.utilities.animate)

local UIListLayout = require(project.simpleElements.UIListLayout)
local SO = Enum.SortOrder
local FD = Enum.FillDirection
local HA = Enum.HorizontalAlignment
local VA = Enum.VerticalAlignment

return function(tabProperty)
    local selectedTab = data.find("selectedTab")

    local isSelected = value(false)

    observe(selectedTab.data):onChange(function()
        local selected = get(selectedTab.data)
        if selected.uid == tabProperty.uid then
            isSelected:set(true)
        else
            isSelected:set(false)
        end
    end)

    return new "Frame" {
        Visible = computed(function()
            if get(isSelected) then
                return true
            end
            return false
        end),
        Name = tabProperty.uid,
		Parent = tabProperty.Parent,
        Size = UDim2.new(1,0,0,28),
        BackgroundTransparency = 1,
        ZIndex = 0,
        AutomaticSize = Enum.AutomaticSize.Y,

        [children] = {
            UIListLayout(SO.LayoutOrder, FD.Vertical,HA.Center,VA.Top)
        }
    }
end
end)() end,
    [72] = function()local wax,script,require=ImportGlobals(72)local ImportGlobals return (function(...)local project = script.Parent.Parent

local services = require(project.Bundles.services)
local ContextActionService = services.ContextActionService
local data = require(project.Bundles.data)
local icons = require(project.Bundles.icons)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed
local observe = Fusion.Observer
local ref = Fusion.Ref
local onevent = Fusion.OnEvent

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local animate = require(project.utilities.animate)
local customFont = require(project.utilities.customFont)

local UIPadding = require(project.simpleElements.UIPadding)
local UICorner = require(project.simpleElements.UICorner)

return function(textboxProperty)
	assert(textboxProperty.Name, ":TextBox missing property Name")
	assert(typeof(textboxProperty.Name) == "string", ("Name accepts type string got %s"):format(typeof(textboxProperty.Name)))
	assert(textboxProperty.Callback, ":TextBox missing property Callback")
	assert(typeof(textboxProperty.Callback) == "function", ("Callback accepts type function got %s"):format(typeof(textboxProperty.Callback)))
	if textboxProperty.Default then
		assert(typeof(textboxProperty.Default) == "string", ("Default accepts type string got %s"):format(typeof(textboxProperty.Default)))
	end
	if textboxProperty.PlaceHolderText then
		assert(typeof(textboxProperty.PlaceHolderText) == "string", ("PlaceHolderText accepts type string got %s"):format(typeof(textboxProperty.PlaceHolderText)))
	end
	if textboxProperty.OnlyNumbers then
		assert(typeof(textboxProperty.OnlyNumbers) == "boolean", ("OnlyNumbers accepts type boolean got %s"):format(typeof(textboxProperty.OnlyNumbers)))
	end
	if textboxProperty.OnLeave then
		assert(typeof(textboxProperty.OnLeave) == "boolean", ("OnLeave accepts type boolean got %s"):format(typeof(textboxProperty.OnLeave)))
	end
	if textboxProperty.Description then
		assert(typeof(textboxProperty.Description) == "string", ("Description accepts type string got %s"):format(typeof(textboxProperty.Description)))
	end
	if textboxProperty.IsEnabled then
		assert(typeof(textboxProperty.IsEnabled) == "boolean", ("IsEnabled accepts type boolean got %s"):format(typeof(textboxProperty.IsEnabled)))
	end
	if textboxProperty.HasPreview then
		assert(typeof(textboxProperty.HasPreview) == "boolean", ("HasPreview accepts type boolean got %s"):format(typeof(textboxProperty.HasPreview)))
	end
	if textboxProperty.PreviewAssetId then
		assert(typeof(textboxProperty.PreviewAssetId) == "string", ("PreviewAssetId accepts type string got %s"):format(typeof(textboxProperty.PreviewAssetId)))
	end
	if textboxProperty.ImageSize then
		assert(typeof(textboxProperty.ImageSize) == "UDim2", ("ImageSize accepts type UDim2 got %s"):format(typeof(textboxProperty.ImageSize)))
	end

	local name = value(textboxProperty.Name)
	local callback = value(textboxProperty.Callback)
	local defaultText = value(textboxProperty.Default or "")
	local numbersOnly = value(textboxProperty.OnlyNumbers or false)
	local onLeave = value(textboxProperty.OnLeave or false)
	local placeHolderText = value(textboxProperty.PlaceHolderText or "")
	local isEnabled = value(textboxProperty.IsEnabled)
	local disabledText = value(textboxProperty.DisabledText or "Component Disabled")

	local previewAssetId = value(textboxProperty.PreviewAssetId)
	local imageSize = value(textboxProperty.ImageSize)

	local currentIndex = data.find(textboxProperty.uid)
	local selectedTab = data.find("selectedTab")
	local hoveredComponent = data.find("hoveredComponent")
	local isHidden = data.find("isHidden")
	local keys = data.find("keys")
	local textboxUID = data.add({}).uid

	local isHovered = value(false)
	observe(currentIndex.index):onChange(function()
		local index = get(currentIndex.index)
		if index == textboxProperty.index then
			isHovered:set(true)
			hoveredComponent:update("data", textboxProperty)
		else
			isHovered:set(false)
		end
	end)

	local textboxRef = value()
	local updatedText = value("")
	local focused = value(false)

	local lastCorrectText = value(get(updatedText))
	observe(updatedText):onChange(function()
		if get(numbersOnly) then
			if tonumber(get(updatedText)) or get(updatedText) == "" then
				lastCorrectText:set(get(updatedText))
			end
		else
			lastCorrectText:set(get(updatedText))
		end
		local textBox = get(textboxRef)
		textBox.Text = get(lastCorrectText)
	end)
	if get(onLeave) then
		observe(focused):onChange(function()
			if not get(focused) then
				if get(numbersOnly) then
					if tonumber(get(updatedText)) or get(updatedText) == "" then
						task.spawn(get(callback), get(updatedText))
					end
				else
					task.spawn(get(callback), get(updatedText))
				end
			end
		end)
	else
		observe(updatedText):onChange(function()
			if get(numbersOnly) then
				if tonumber(get(updatedText)) or get(updatedText) == "" then
					task.spawn(get(callback), get(updatedText))
				end
			else
				task.spawn(get(callback), get(updatedText))
			end
		end)
	end
	if get(defaultText) ~= "" then
		updatedText:set(get(defaultText))
	end

	local textboxComponent = new "Frame" {
		Parent = textboxProperty.Parent,
		Size = UDim2.new(1,0,0,28),
		BackgroundColor3 = theme.get('selectedTab', true),
		BackgroundTransparency = animate(function()
			if get(isHovered) then
				return 0.55
			end
			return 1
		end,40,1),

		[children] = {
			new "ImageLabel" { -- Preview
				Visible = computed(function()
					if textboxProperty.HasPreview then
						return true
					end
					return false
				end),
				BackgroundColor3 = theme.get("background", true),
				Image = computed(function()
					return get(previewAssetId)
				end),
				BackgroundTransparency = animate(function()
					if get(isHovered) then
						return 0.65
					end
					return 1
				end,40,1),
				ImageTransparency = animate(function()
					if get(isHovered) then
						return 0
					end
					return 1
				end,40,1),
				Position = UDim2.new(1,5,0,0),
				Size = computed(function()
					return get(imageSize)
				end)
			},
			new "Frame" { -- Disabled Frame
				ZIndex = 2,
				Visible = computed(function()
					if get(isEnabled) ~= nil and not get(isEnabled)  then
						return true
					end
					return false
				end),
				BackgroundTransparency = 0.4,
				BackgroundColor3 = theme.get("background", true),
				Size = UDim2.fromScale(1,1),

				[children] = {
					new "ImageLabel" { -- Icon
						Size = UDim2.fromOffset(24,24),
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,5,0.5,0),
						BackgroundTransparency = 1,
						Image = icons.lock,
						ImageColor3 = theme.get("text", true)
					},
					new "TextLabel" {
						FontFace = customFont("Inter", Enum.FontWeight.Bold),
						BackgroundTransparency = 1,
						Size = UDim2.new(1,-24,0,14),
						TextColor3 = theme.get("text", true),
						TextScaled = true,
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,29,0.5,0),
						Text = computed(function()
							return get(disabledText)
						end),
						TextXAlignment = Enum.TextXAlignment.Left,
						RichText = true,

						[children] = {
							UIPadding(0,5)
						}
					}
				}
			},
			new "TextLabel" { -- Name
				FontFace = customFont("Inter", Enum.FontWeight.Medium),
				Size = UDim2.new(0.6,-5,0,14),
				BackgroundTransparency = 1,
				TextColor3 = theme.get("text", true),
				TextScaled = true,
				AnchorPoint = Vector2.new(0,0.5),
				Position = UDim2.new(0,0,0.5,0),
				Text = computed(function()
					return get(name)
				end),
				TextXAlignment = Enum.TextXAlignment.Left,
				RichText = true,

				[children] = {
					UIPadding(0,0,0,5)
				}
			},
			new "Frame" { -- TextBox
				Size = UDim2.fromScale(0.4,1),
				BackgroundTransparency = 1,
				AnchorPoint = Vector2.new(1,0.5),
				Position = UDim2.new(1,0,0.5,0),

				[children] = {
					UIPadding(5,5,5,0),
					new "Frame" {
						Size = UDim2.fromScale(1,1),
						BackgroundTransparency = 0.65,
						BackgroundColor3 = theme.get("background", true),

						[children] = {
							UICorner(4),
							new "TextBox" {
								Interactable = computed(function()
									return get(focused)
								end),
								Size = UDim2.new(1,0,0,12),
								AnchorPoint = Vector2.new(0,0.5),
								Position = UDim2.fromScale(0,0.5),
								BackgroundTransparency = 1,
								FontFace = customFont("Inter", Enum.FontWeight.Medium),
								TextColor3 = theme.get("toggled", true),
								PlaceholderColor3 = theme.get("text2", true),
								PlaceholderText = get(placeHolderText),
								TextXAlignment = Enum.TextXAlignment.Left,
								TextTransparency = animate(function()
									local textbox = get(textboxRef)
									if typeof(textbox) == "Instance" and textbox.Text and textbox.Text == "" then
										if get(focused) then
											return 0
										end
										return 0.4
									end
									return 0
								end,40,1),
								TextScaled = true,

								[onevent "FocusLost"] = function()
									focused:set(false)
								end,
								[onevent "Changed"] = function(changedProperty)
									local textbox = get(textboxRef)
									if changedProperty == "Text" and textbox.Text and textbox.Text ~= get(lastCorrectText) then
										updatedText:set(textbox.Text)
									end
								end,

								[ref] = textboxRef,

								[children] = {
									UIPadding(0,5)
								}
							}
						}
					}
				}
			}
		}
	}

	ContextActionService:BindActionAtPriority(textboxUID.." Focus", function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		if get(isHidden.boolean) or get(focused) or not get(isHovered) or get(selectedTab.data).uid ~= textboxProperty.uid or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		focused:set(true)
		local textbox = get(textboxRef)
		task.wait()
		textbox:CaptureFocus()
		return Enum.ContextActionResult.Sink
	end, false, Enum.ContextActionPriority.High.Value, get(keys.key).enter)

	if get(currentIndex.index) == textboxProperty.index then
		isHovered:set(true)
	end

	local returnFunctions = {}
	function returnFunctions:SetInput(newInput)
		if get(numbersOnly) then
			assert(typeof(newInput) == "number", (":SetInput accepts type number got %s"):format(typeof(newInput)))
		else
			assert(typeof(newInput) == "string", (":SetInput accepts type string got %s"):format(typeof(newInput)))
		end
		updatedText:set(newInput)
	end
	function returnFunctions:SetPreviewAssetId(newPreviewAssetId)
		previewAssetId:set(newPreviewAssetId)
	end
	function returnFunctions:SetImageSize(newImageSize)
		imageSize:set(newImageSize)
	end
	return returnFunctions
end
end)() end,
    [73] = function()local wax,script,require=ImportGlobals(73)local ImportGlobals return (function(...)local project = script.Parent.Parent

local data = require(project.Bundles.data)
local services = require(project.Bundles.services)
local ContextActionService = services.ContextActionService
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local children = Fusion.Children
local value = Fusion.Value
local computed = Fusion.Computed
local observe = Fusion.Observer

local get = require(project.utilities.get)
local theme = require(project.Bundles.themeSystem)
local animate = require(project.utilities.animate)
local icons = require(project.Bundles.icons)
local customFont = require(project.utilities.customFont)

local UICorner = require(project.simpleElements.UICorner)
local UIPadding = require(project.simpleElements.UIPadding)

return function(toggleProperty)
	assert(toggleProperty.Name, ":Toggle missing property Name")
	assert(typeof(toggleProperty.Name) == "string", ("Name accepts type string got %s"):format(typeof(toggleProperty.Name)))
	assert(toggleProperty.Callback, ":Toggle missing property Callback")
	assert(typeof(toggleProperty.Callback) == "function", ("Callback accepts type function got %s"):format(typeof(toggleProperty.Callback)))
	if toggleProperty.Description then
		assert(typeof(toggleProperty.Description) == "string", ("Description accepts type string got %s"):format(typeof(toggleProperty.Description)))
	end
	if toggleProperty.IsEnabled then
		assert(typeof(toggleProperty.IsEnabled) == "boolean", ("IsEnabled accepts type boolean got %s"):format(typeof(toggleProperty.IsEnabled)))
	end
	if toggleProperty.HasPreview then
		assert(typeof(toggleProperty.HasPreview) == "boolean", ("HasPreview accepts type boolean got %s"):format(typeof(toggleProperty.HasPreview)))
	end
	if toggleProperty.PreviewAssetId then
		assert(typeof(toggleProperty.PreviewAssetId) == "string", ("PreviewAssetId accepts type string got %s"):format(typeof(toggleProperty.PreviewAssetId)))
	end
	if toggleProperty.ImageSize then
		assert(typeof(toggleProperty.ImageSize) == "UDim2", ("ImageSize accepts type UDim2 got %s"):format(typeof(toggleProperty.ImageSize)))
	end

	local name = value(toggleProperty.Name)
	local callback = value(toggleProperty.Callback)
	local isEnabled = value(toggleProperty.IsEnabled)
	local disabledText = value(toggleProperty.DisabledText or "Component Disabled")

	local previewAssetId = value(toggleProperty.PreviewAssetId)
	local imageSize = value(toggleProperty.ImageSize)

	local currentIndex = data.find(toggleProperty.uid)
	local selectedTab = data.find("selectedTab")
	local isHidden = data.find("isHidden")
	local hoveredComponent = data.find("hoveredComponent")
	local keys = data.find("keys")
	local toggleUID = data.add({}).uid

	local isHovered = value(false)
	observe(currentIndex.index):onChange(function()
		local index = get(currentIndex.index)
		if index == toggleProperty.index then
			isHovered:set(true)
			hoveredComponent:update("data", toggleProperty)
		else
			isHovered:set(false)
		end
	end)

	local isToggled = value(false)

	local toggleComponent = new "Frame" {
		Parent = toggleProperty.Parent,
		Size = UDim2.new(1,0,0,28),
		BackgroundColor3 = theme.get('selectedTab', true),
		BackgroundTransparency = animate(function()
			if get(isHovered) then
				return 0.55
			end
			return 1
		end,40,1),

		[children] = {
			new "ImageLabel" { -- Preview
				Visible = computed(function()
					if toggleProperty.HasPreview then
						return true
					end
					return false
				end),
				BackgroundColor3 = theme.get("background", true),
				Image = computed(function()
					return get(previewAssetId)
				end),
				BackgroundTransparency = animate(function()
					if get(isHovered) then
						return 0.65
					end
					return 1
				end,40,1),
				ImageTransparency = animate(function()
					if get(isHovered) then
						return 0
					end
					return 1
				end,40,1),
				Position = UDim2.new(1,5,0,0),
				Size = computed(function()
					return get(imageSize)
				end)
			},
			new "Frame" { -- Disabled Frame
				ZIndex = 2,
				Visible = computed(function()
					if get(isEnabled) ~= nil and not get(isEnabled)  then
						return true
					end
					return false
				end),
				BackgroundTransparency = 0.4,
				BackgroundColor3 = theme.get("background", true),
				Size = UDim2.fromScale(1,1),

				[children] = {
					new "ImageLabel" { -- Icon
						Size = UDim2.fromOffset(24,24),
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,5,0.5,0),
						BackgroundTransparency = 1,
						Image = icons.lock,
						ImageColor3 = theme.get("text", true)
					},
					new "TextLabel" {
						FontFace = customFont("Inter", Enum.FontWeight.Bold),
						BackgroundTransparency = 1,
						Size = UDim2.new(1,-24,0,14),
						TextColor3 = theme.get("text", true),
						TextScaled = true,
						AnchorPoint = Vector2.new(0,0.5),
						Position = UDim2.new(0,29,0.5,0),
						Text = computed(function()
							return get(disabledText)
						end),
						TextXAlignment = Enum.TextXAlignment.Left,
						RichText = true,

						[children] = {
							UIPadding(0,5)
						}
					}
				}
			},
			new "TextLabel" { -- Name
				FontFace = customFont("Inter", Enum.FontWeight.Medium),
				Size = UDim2.new(1,-39,0,14),
				BackgroundTransparency = 1,
				TextColor3 = theme.get("text", true),
				TextScaled = true,
				AnchorPoint = Vector2.new(0,0.5),
				Position = UDim2.new(0,0,0.5,0),
				Text = computed(function()
					return get(name)
				end),
				TextXAlignment = Enum.TextXAlignment.Left,
				RichText = true,

				[children] = {
					UIPadding(0,0,0,5)
				}
			},
			new "Frame" { -- Toggle
				AnchorPoint = Vector2.new(1,0.5),
				Position = UDim2.new(1,-5,0.5,0),
				Size = UDim2.new(0,32,0,18),
				BackgroundColor3 = animate(function()
					if get(isToggled) then
						return theme.get("toggled")
					end
					return theme.get("background")
				end,60,1),
				BackgroundTransparency = animate(function()
					if get(isToggled) then
						return 0.2
					end
					return 0.65
				end,60,1),

				[children] = {
					UICorner(1000),
					new "ImageLabel" {
						AnchorPoint = Vector2.new(0.5,0.5),
						Position = animate(function()
							if get(isToggled) then
								return UDim2.fromScale(0.73,0.5)
							end
							return UDim2.fromScale(0.27,0.5)
						end,40,1),
						Size = UDim2.fromOffset(16,16),
						BackgroundTransparency = 1,
						Image = icons.circle,
						ImageColor3 = theme.get("text",true),
						ImageTransparency = 0
					}
				}
			}
		}
	}

	ContextActionService:BindActionAtPriority(toggleUID, function(_, inputState)
		if inputState ~= Enum.UserInputState.Begin then
			return Enum.ContextActionResult.Pass
		end
		print("Toggle:", _, inputState)
		if get(isHidden.boolean) or not get(isHovered) or get(selectedTab.data).uid ~= toggleProperty.uid or (get(isEnabled) ~= nil and not get(isEnabled)) then
			return Enum.ContextActionResult.Pass
		end
		isToggled:set(not get(isToggled))
		task.spawn(get(callback), get(isToggled))
		return Enum.ContextActionResult.Sink
	end,false, Enum.ContextActionPriority.High.Value, get(keys.key).enter)
	if get(currentIndex.index) == toggleProperty.index then
		isHovered:set(true)
	end

	local returnFunctions = {}
	function returnFunctions:SetValue(newValue)
		isToggled:set(newValue)
		task.spawn(get(callback), get(isToggled))
	end
	function returnFunctions:SetPreviewAssetId(newPreviewAssetId)
		previewAssetId:set(newPreviewAssetId)
	end
	function returnFunctions:SetImageSize(newImageSize)
		imageSize:set(newImageSize)
	end
	return returnFunctions
end
end)() end,
    [74] = function()local wax,script,require=ImportGlobals(74)local ImportGlobals return (function(...)--[[]]
local project = require(script.Parent)
local randomString = require(script.Parent.utilities.randomString)
local lucide = require(script.Parent.Bundles.lucide)

local randomIcons = {}
for _,v in lucide do
    table.insert(randomIcons, v)
end
local function randomIcon()
    return randomIcons[math.floor(math.random(1,#randomIcons))]
end


return function(target)
    local window = project:Window({
        Title = "Menu",
        FooterLeft = "V1",
        FooterRight = "griffindoescooking",
        Image = "rbxassetid://18947191044",
        ImageBackgroundColor = Color3.fromHex("#50579B"),
        HideBind = Enum.KeyCode.T,
        BackBind = Enum.KeyCode.Backspace,
        UpBind = Enum.KeyCode.Up,
        DownBind = Enum.KeyCode.Down,
        EnterBind = Enum.KeyCode.Four,
        LeftBind = Enum.KeyCode.Q,
        RightBind = Enum.KeyCode.E,

        noOffset = true,
        Parent = target,
    })


    local tab = window:Tab({
        Name = randomString((6+3)*3),
        Image = randomIcon()
    })

    local button = tab:Button({
        Name = "Interact With Me!",
        Description = "This will callback on Return",
        Callback = function()
            print("we got callback boys")
        end,

        IsEnabled = false,
        DisabledText = "Hey you cant use this!",

        HasPreview = true,
        PreviewAssetId = "rbxassetid://10723424235",
        ImageSize = UDim2.fromOffset(48,48)
    })
    task.delay(2, function()
        button:SetPreviewAssetId(randomIcon())
        button:SetImageSize(UDim2.fromOffset(200,200))
    end)
    tab:Toggle({
        Name = "Toggle Me!",
        Description = "Hey this is a place for description text that will have\nnew lines within it",
        Callback = function(value)
            print(value)
        end,

        --IsEnabled = false,
        DisabledText = "Hey you cant use this!",

        HasPreview = true,
        PreviewAssetId = "rbxassetid://10723424235",
        ImageSize = UDim2.fromOffset(48,48)
    })
    local dropdown = tab:Dropdown({
        Name = "Single Item Selection",
        Items = {{
            Image = lucide.apple,
            Value = "Apple",
            HasPreview = true,
            PreviewAssetId = "rbxassetid://10723424235",
            ImageSize = UDim2.fromOffset(48,48)
        }, "Banana", "Carrot", "Dingleberry", "Eggplant", "Fruit", "Grape", "Hen", "India", "Jumprope", "Kite", "Lime","Music","Number","Omega","Pencil","Quadrant", "Rust"},
        Multiselect = false,
        Callback = function(value)
            print(value)
        end,

        --IsEnabled = false,
        --DisabledText = "Hey you cant use this!",

        HasPreview = true,
        PreviewAssetId = "rbxassetid://18984322365",
        ImageSize = UDim2.fromOffset(200,300)
    })
    local dropdown2 = tab:Dropdown({
        Name = "Multi Item Selection",
        Items = {{
            --Image = lucide.apple,
            Value = "Apple",
            HasPreview = true,
            PreviewAssetId = "rbxassetid://10723424235",
            ImageSize = UDim2.fromOffset(48,48)
        }, "Banana", "Carrot", "Dingleberry", "Eggplant", "Fruit", "Grape", "Hen", "India", "Jumprope", "Kite", "Lime","Music","Number","Omega","Pencil","Quadrant", "Rust"},
        Multiselect = true,
        Callback = function(value)
            print(value)
        end,

        HasPreview = true,
        PreviewAssetId = "rbxassetid://10723424235",
        ImageSize = UDim2.fromOffset(48,48)
    })
    tab:Label("HEY IM A REALLY LONG PIECE OF TEXT WITH MULTIPLE SPACES WITHIN IT RESIZE MEEEEE")
    tab:TextBox({
        Name = "Enter to type below",
        Default = "",
        OnlyNumbers = true,
        OnLeave = false,
        PlaceHolderText = "Base Size",
        Callback = function(text)
            print('output: "'..text..'"')
        end,

        HasPreview = true,
        PreviewAssetId = "rbxassetid://10723424235",
        ImageSize = UDim2.fromOffset(48,48)
    })
    tab:Slider({
        Name = "Slide Me!",
        Min = 0,
        Max = 100,
        Increment = 5,
        UseHold = true,
        DisplayAsPercent = true,
        Callback = function(value)
            print(value)
        end,

        HasPreview = true,
        PreviewAssetId = "rbxassetid://10723424235",
        ImageSize = UDim2.fromOffset(48,48)
    })
    tab:Keybind({
        Name = "Binded Action",
        Bind = Enum.KeyCode.F,
        Callback = function()
            print('Binded Action Occurred')
        end,

        HasPreview = true,
        PreviewAssetId = "rbxassetid://10723424235",
        ImageSize = UDim2.fromOffset(48,48)
    })
    tab:Keybind({
        Name = "Binded Action",
        Bind = Enum.KeyCode.X,
        IsToggle = true,
        Callback = function(value)
            print('Binded Action Occurred:', value)
        end,

        HasPreview = true,
        PreviewAssetId = "rbxassetid://10723424235",
        ImageSize = UDim2.fromOffset(48,48)
    })

    task.delay(2, function()
        for i=1,12 do
            project:Notify({
                Title = "the title",
                Body = "body\nblah\nblah\nblah",
                Duration = 10,
                Image = randomIcon()
            })
            task.wait(1)
        end
    end)


    return function()
        project:Destroy()
    end
end

end)() end,
    [76] = function()local wax,script,require=ImportGlobals(76)local ImportGlobals return (function(...)local project = script.Parent.Parent

local data = require(project.Bundles.data)
local Fusion = require(project.Bundles.Fusion)
local new = Fusion.New
local onevent = Fusion.OnEvent
local children = Fusion.Children
local value = Fusion.Value
local ref = Fusion.Ref
local computed = Fusion.Computed


local UIListLayout = require(project.simpleElements.UIListLayout)
local UIPadding = require(project.simpleElements.UIPadding)
local SO = Enum.SortOrder
local FD = Enum.FillDirection
local HA = Enum.HorizontalAlignment
local VA = Enum.VerticalAlignment

local get = require(project.utilities.get)
local animate = require(project.utilities.animate)
local theme = require(project.Bundles.themeSystem)
local references = require(project.utilities.references)
local customFont = require(project.utilities.customFont)
local drag = require(project.utilities.drag)
local connections = require(project.utilities.connections)
local randomString = require(project.utilities.randomString)

return function(gtaMenuProperty)
	assert(gtaMenuProperty.Title, ":Window missing property Title")
	assert(typeof(gtaMenuProperty.Title) == "string", ("Title accepts string got %s"):format(typeof(gtaMenuProperty.Title)))

	local containerX = value(gtaMenuProperty.SizeXInOffset or 300)

	-- References

	local container = value()
	references.add(container)
	local tabButtonList = value()
	references.add(tabButtonList)
	local tabWindowList = value()
	references.add(tabWindowList)
	local dropdownLists = value()
	references.add(dropdownLists)
	local notificationList = value()
	references.add(notificationList)
	local gtaMenuRef = value()
	references.add(gtaMenuRef)

	-- stuff

	local selectedTab = data.find("selectedTab")
	local hoveredComponent = data.find("hoveredComponent")
	local isHidden = data.find("isHidden")
	local absoluteOffset = data.find("absoluteOffset")

	local gtaMenu = new "ScreenGui" {
		Name = randomString(32),
		Parent = gtaMenuProperty.Parent,
		ResetOnSpawn = false,
		IgnoreGuiInset = true,
		Enabled = computed(function()
			if get(isHidden.boolean) then
				return false
			end
			return true
		end),

		[ref] = gtaMenuRef,

		[children] = {
			new "Frame" {
				Name = "container",

				Position = computed(function()
					return UDim2.new(0,10,0,10 + get(absoluteOffset.amount))
				end),
				Size = animate(function()
					return UDim2.fromOffset(get(containerX),0)
				end,20,1),
				BackgroundColor3 = theme.get("background", true),
				BackgroundTransparency = 0.65,
				AutomaticSize = Enum.AutomaticSize.Y,

				[ref] = container,

				[onevent "Destroying"] = function()
					connections.deleteConnections()
				end,

				[children] = {
					new "Frame" {
						Size = computed(function()
							return UDim2.fromOffset(get(containerX),0)
						end),
						BackgroundTransparency = 1,
						AutomaticSize = Enum.AutomaticSize.Y,

						[children] = {
							UIListLayout(SO.LayoutOrder,FD.Vertical,HA.Center,VA.Top),
							new "ImageLabel" {
								Size = UDim2.new(1,0,0,90),
								AnchorPoint = Vector2.new(0.5,0.5),
								Position = UDim2.fromScale(0.5,0.5),
								Image = gtaMenuProperty.Image,
								BackgroundColor3 = gtaMenuProperty.ImageBackgroundColor,
								ScaleType = Enum.ScaleType.Fit
							},
							new "Frame" {
								BackgroundColor3 = theme.get("background", true),
								Size = UDim2.new(1,0,0,24),

								[children] = {
									UIPadding(0,0,0,5),
									new "TextLabel" {
										Size = UDim2.new(1,0,0,16),
										BackgroundTransparency = 1,
										AnchorPoint = Vector2.new(0,0.5),
										Position = UDim2.fromScale(0,0.5),
										FontFace = customFont("Inter", Enum.FontWeight.Bold),
										TextColor3 = theme.get("text", true),
										Text = computed(function()
											return get(selectedTab.data).Name or get(selectedTab.data).data and get(get(selectedTab.data).data).Name or gtaMenuProperty.Title
										end),
										TextXAlignment = Enum.TextXAlignment.Left,
										TextScaled = true,
										RichText = true,
									}
								}
							},
							new "Frame" {
								Size = UDim2.new(1,0,0,0),
								AutomaticSize = Enum.AutomaticSize.Y,
								BackgroundTransparency = 1,

								[children] = {
									UIListLayout(SO.LayoutOrder,FD.Horizontal,HA.Left,VA.Top, UDim.new(0,10)),
									new "Frame" {
										Visible = computed(function()
											if get(selectedTab.data).uid then
												return false
											end
											return true
										end),
										Name = "tabButtonList",
										BackgroundTransparency = 1,
										Size = UDim2.new(1,0,1,0),
										AutomaticSize = Enum.AutomaticSize.Y,

										[ref] = tabButtonList,

										[children] = {
											UIListLayout(SO.LayoutOrder, FD.Vertical, HA.Center, VA.Top)
										}
									},
									new "Frame" {
										Visible = computed(function()
											if not get(selectedTab.data).uid then
												return false
											end
											return true
										end),
										Name = "tabWindowList",
										BackgroundTransparency = 1,
										Size = UDim2.new(1,0,0,0),
										AutomaticSize = Enum.AutomaticSize.Y,

										[ref] = tabWindowList,
									}
								}
							},
							new "Frame" {
								BackgroundColor3 = theme.get("background", true),
								Size = UDim2.new(1,0,0,24),

								[children] = {
									UIPadding(0,5,0,5),
									new "TextLabel" {
										Size = UDim2.new(1,0,0,12),
										BackgroundTransparency = 1,
										AnchorPoint = Vector2.new(0,0.5),
										Position = UDim2.fromScale(0,0.5),
										FontFace = customFont("Inter", Enum.FontWeight.Medium),
										TextColor3 = theme.get("text", true),
										Text = computed(function()
											if get(hoveredComponent.data).Description then
												return get(hoveredComponent.data).Description
											end
											return gtaMenuProperty.FooterLeft
										end),
										TextXAlignment = Enum.TextXAlignment.Left,
										TextScaled = true,
										RichText = true,
									},
									new "TextLabel" {
										Visible = computed(function()
											if get(hoveredComponent.data).Description then
												return false
											end
											return true
										end),
										Size = UDim2.new(1,0,0,12),
										BackgroundTransparency = 1,
										AnchorPoint = Vector2.new(0,0.5),
										Position = UDim2.fromScale(0,0.5),
										FontFace = customFont("Inter", Enum.FontWeight.Medium),
										TextColor3 = theme.get("text", true),
										Text = gtaMenuProperty.FooterRight,
										TextXAlignment = Enum.TextXAlignment.Right,
										TextScaled = true,
										RichText = true,
									}
								}
							}
						}
					}
				}
			},
			new "ScrollingFrame" {
				Name = "notificationList",
				BackgroundTransparency = 1,
				Size = UDim2.new(0,2,1,-20),
				Position = UDim2.new(1,-5,0.5,0),
				AnchorPoint = Vector2.new(1,0.5),
				ScrollBarThickness = 0,
				CanvasSize = UDim2.new(0,0,1,0),
				AutomaticCanvasSize = Enum.AutomaticSize.Y,
				ClipsDescendants = false,

				[ref] = notificationList,

				[children] = {
					UIListLayout(SO.LayoutOrder,FD.Vertical,HA.Right,VA.Bottom, UDim.new(0,5))
				}
			}
		}
	}

	drag(references.get("container"), 15)

	return gtaMenu
end
end)() end,
    [77] = function()local wax,script,require=ImportGlobals(77)local ImportGlobals return (function(...)return {
	defaultTab = Color3.fromHex("#a49ae6"),
	background = Color3.fromRGB(40, 44, 50),
	secondaryBackground = Color3.fromRGB(49, 56, 66),
	tertiaryBackground = Color3.fromRGB(57, 63, 75),
	text = Color3.fromRGB(220,221,225),
	image = Color3.fromRGB(220,221,225),
	placeholder = Color3.fromRGB(165,166,169),
	close = Color3.fromRGB(190, 100, 105)
}
end)() end,
    [79] = function()local wax,script,require=ImportGlobals(79)local ImportGlobals return (function(...)local Fusion = require(script.Parent.Parent.Bundles.Fusion)

return function(AspectRatio:number, AspectType:Enum.AspectType, DominantAxis:Enum.DominantAxis)
    AspectType = AspectType or Enum.AspectType.FitWithinMaxSize
    DominantAxis = DominantAxis or Enum.DominantAxis.Height
    return Fusion.New "UIAspectRatioConstraint" {
        AspectRatio = AspectRatio,
        AspectType = AspectType,
        DominantAxis = DominantAxis
    }
end
end)() end,
    [80] = function()local wax,script,require=ImportGlobals(80)local ImportGlobals return (function(...)local Fusion = require(script.Parent.Parent.Bundles.Fusion)

return function(offset)
    return Fusion.New "UICorner" {
        CornerRadius = UDim.new(0, offset)
    }
end
end)() end,
    [81] = function()local wax,script,require=ImportGlobals(81)local ImportGlobals return (function(...)local Fusion = require(script.Parent.Parent.Bundles.Fusion)

return function(SortOrder, FillDirection,HorizontalAlignment,VerticalAlignment,Padding)
    return Fusion.New "UIListLayout" {
        SortOrder = SortOrder or Enum.SortOrder.LayoutOrder,
        FillDirection = FillDirection or Enum.FillDirection.Vertical,
        HorizontalAlignment = HorizontalAlignment or Enum.HorizontalAlignment.Center,
        VerticalAlignment = VerticalAlignment or Enum.VerticalAlignment.Center,
        Padding = Padding or UDim.new(0,0)
    }
end
end)() end,
    [82] = function()local wax,script,require=ImportGlobals(82)local ImportGlobals return (function(...)local Fusion = require(script.Parent.Parent.Bundles.Fusion)

return function(...)
    local args = {...}
    if #args == 1 then
        return Fusion.New "UIPadding" {
            PaddingLeft = UDim.new(0,...),
            PaddingRight = UDim.new(0,...),
            PaddingTop = UDim.new(0,...),
            PaddingBottom = UDim.new(0,...),
        }
    elseif #args == 2 then
        return Fusion.New "UIPadding" {
            PaddingTop = UDim.new(0,args[1]),
            PaddingRight = UDim.new(0,args[2]),
            PaddingBottom = UDim.new(0,args[1]),
            PaddingLeft = UDim.new(0,args[2]),
        }
    elseif #args == 3 then
        return Fusion.New "UIPadding" {
            PaddingTop = UDim.new(0,args[1]),
            PaddingRight = UDim.new(0,args[2]),
            PaddingBottom = UDim.new(0,args[3]),
            PaddingLeft = UDim.new(0,args[2]),
        }
    else
        return Fusion.New "UIPadding" {
            PaddingTop = UDim.new(0,args[1]),
            PaddingRight = UDim.new(0,args[2]),
            PaddingBottom = UDim.new(0,args[3]),
            PaddingLeft = UDim.new(0,args[4]),
        }
    end
end
end)() end,
    [84] = function()local wax,script,require=ImportGlobals(84)local ImportGlobals return (function(...)local Fusion = require(script.Parent.Parent.Bundles.Fusion)
local computed,spring = Fusion.Computed,Fusion.Spring

return function(callback, speed, damping)
	return spring(computed(callback), speed, damping)
end
end)() end,
    [85] = function()local wax,script,require=ImportGlobals(85)local ImportGlobals return (function(...)local connections = {}
local connectionsFunction = {}

local services = require(script.Parent.Parent.Bundles.services)
local ContextActionService = services.ContextActionService

function connectionsFunction.add(connection)
    table.insert(connections, connection)
end
function connectionsFunction.deleteConnections()
    for i,v in connections do
        v:Disconnect()
        connections[i] = nil
    end
    ContextActionService:UnbindAllActions()
end

return connectionsFunction
end)() end,
    [86] = function()local wax,script,require=ImportGlobals(86)local ImportGlobals return (function(...)local fonts = {
    ["Inter"] = "rbxassetid://12187365364"
}

return function(font:string, weight:Enum.FontWeight?, style:Enum.FontStyle?)
    assert(font, "font required")
    weight = weight or Enum.FontWeight.Regular
    style = style or Enum.FontStyle.Normal
    if fonts[font] then
        return Font.new(fonts[font],weight,style)
    else
        error("no font:", font)
    end
end
end)() end,
    [87] = function()local wax,script,require=ImportGlobals(87)local ImportGlobals return (function(...)local functions = {}

function functions.cloneref(service)
    if cloneref then
        return cloneref(service)
    else
        return service
    end
end

functions.getgenv = getgenv or nil
return functions
end)() end,
    [88] = function()local wax,script,require=ImportGlobals(88)local ImportGlobals return (function(...)return function(o,s)
	local services = require(script.Parent.Parent.Bundles.services)
	local DRAG_SPEED = s or 10
	local UserInputService = services.UserInputService
	local runService = services.RunService
	local gui = o
	local dragging, lastMousePos, lastGoalPos, startPos
	local function Lerp(a, b, m)
		return a + (b - a) * m
	end
	
	local function Update(dt)
		if not (startPos) then return end;
		
		if not (dragging) and (lastGoalPos) then
			gui.Position = UDim2.new(startPos.X.Scale, Lerp(gui.Position.X.Offset, lastGoalPos.X.Offset, dt * DRAG_SPEED), startPos.Y.Scale, Lerp(gui.Position.Y.Offset, lastGoalPos.Y.Offset, dt * DRAG_SPEED))
			return 
		end
		
		local delta = (lastMousePos - UserInputService:GetMouseLocation())
		
		local xGoal = (startPos.X.Offset - delta.X);
		local yGoal = (startPos.Y.Offset - delta.Y);
		
		lastGoalPos = UDim2.new(startPos.X.Scale, xGoal, startPos.Y.Scale, yGoal)
		
		gui.Position = UDim2.new(startPos.X.Scale, Lerp(gui.Position.X.Offset, xGoal, dt * DRAG_SPEED), startPos.Y.Scale, Lerp(gui.Position.Y.Offset, yGoal, dt * DRAG_SPEED))
	end
	
	gui.InputBegan:Connect(function(input)
		if input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch then
			dragging = true
			startPos = gui.Position
			lastMousePos = UserInputService:GetMouseLocation()
			input.Changed:Connect(function()
				if input.UserInputState == Enum.UserInputState.End then
					dragging = false
				end
			end)
		end
	end)
	local connection = runService.Heartbeat:Connect(Update)
	return connection
end

end)() end,
    [89] = function()local wax,script,require=ImportGlobals(89)local ImportGlobals return (function(...)return function(value, dependency)
	if typeof(value) == "table" and value.type == "State" then
		return value:get(dependency)
	end
	return value
end
end)() end,
    [90] = function()local wax,script,require=ImportGlobals(90)local ImportGlobals return (function(...)return function(text:string, properties)
    text = text or ""
    local params = Instance.new("GetTextBoundsParams")
    params.Text = text
    params.Font = properties.Font
    params.Size = properties.Size
    params.Width = properties.Width
    local TextService = require(script.Parent.Parent.Bundles.services).TextService
    return TextService:GetTextBoundsAsync(params)
end
end)() end,
    [91] = function()local wax,script,require=ImportGlobals(91)local ImportGlobals return (function(...)local charset = {}
for i = 48,  57 do table.insert(charset, string.char(i)) end
for i = 65,  90 do table.insert(charset, string.char(i)) end
for i = 97, 122 do table.insert(charset, string.char(i)) end
local function randomString(length)
    if length > 0 then
        return randomString(length - 1) .. charset[math.random(1, #charset)]
    else
        return ""
    end
end
return randomString
end)() end,
    [92] = function()local wax,script,require=ImportGlobals(92)local ImportGlobals return (function(...)local references = {}
local returnFunctions = {}
local observe = require(script.Parent.Parent.Bundles.Fusion).Observer

local get = require(script.Parent.get)

function returnFunctions.add(REF)
    if typeof(get(REF)) ~= "Instance" then
        observe(REF):onChange(function()
            if typeof(get(REF)) == "Instance" then
                if not references[get(REF).Name] then
                    references[get(REF).Name] = get(REF)
                else
                    warn(get(REF).Name, " REF was already added")
                end
            end
        end)
    else
        if not references[get(REF).Name] then
            references[get(REF).Name] = get(REF)
        else
            warn(get(REF).Name, " REF was already added")
        end
    end
end
function returnFunctions.get(name)
    if references[name] then
        return references[name]
    end
end
function returnFunctions.remove(name)
    if references[name] then
        references[name] = nil
    else
        warn(name, "isn't in the references list")
    end
end
function returnFunctions.clear()
    for i,v in references do
        references[i] = nil
        if typeof(v) == "Instance" then
            v:Destroy()
        end
    end
end

return returnFunctions
end)() end,
    [93] = function()local wax,script,require=ImportGlobals(93)local ImportGlobals return (function(...)local Fusion = require(script.Parent.Parent.Bundles.Fusion)
local computed = Fusion.Computed

return function(callback,style)
	return Fusion.Tween(computed(callback), style)
end
end)() end
} -- [RefId] = Closure

-- Holds the actual DOM data
local ObjectTree = {
    {
        1,
        2,
        {
            "gtaMenu"
        },
        {
            {
                83,
                1,
                {
                    "utilities"
                },
                {
                    {
                        88,
                        2,
                        {
                            "drag"
                        }
                    },
                    {
                        86,
                        2,
                        {
                            "customFont"
                        }
                    },
                    {
                        84,
                        2,
                        {
                            "animate"
                        }
                    },
                    {
                        92,
                        2,
                        {
                            "references"
                        }
                    },
                    {
                        89,
                        2,
                        {
                            "get"
                        }
                    },
                    {
                        91,
                        2,
                        {
                            "randomString"
                        }
                    },
                    {
                        85,
                        2,
                        {
                            "connections"
                        }
                    },
                    {
                        93,
                        2,
                        {
                            "tween"
                        }
                    },
                    {
                        90,
                        2,
                        {
                            "getStringBounds"
                        }
                    },
                    {
                        87,
                        2,
                        {
                            "customFunctions"
                        }
                    }
                }
            },
            {
                63,
                1,
                {
                    "components"
                },
                {
                    {
                        66,
                        2,
                        {
                            "keybind"
                        }
                    },
                    {
                        67,
                        2,
                        {
                            "label"
                        }
                    },
                    {
                        70,
                        2,
                        {
                            "tabButton"
                        }
                    },
                    {
                        68,
                        2,
                        {
                            "notification"
                        }
                    },
                    {
                        72,
                        2,
                        {
                            "textbox"
                        }
                    },
                    {
                        69,
                        2,
                        {
                            "slider"
                        }
                    },
                    {
                        65,
                        2,
                        {
                            "dropdown"
                        }
                    },
                    {
                        73,
                        2,
                        {
                            "toggle"
                        }
                    },
                    {
                        64,
                        2,
                        {
                            "button"
                        }
                    },
                    {
                        71,
                        2,
                        {
                            "tabWindow"
                        }
                    }
                }
            },
            {
                2,
                1,
                {
                    "Bundles"
                },
                {
                    {
                        59,
                        2,
                        {
                            "lucide"
                        }
                    },
                    {
                        60,
                        2,
                        {
                            "services"
                        }
                    },
                    {
                        3,
                        2,
                        {
                            "Fusion"
                        },
                        {
                            {
                                33,
                                1,
                                {
                                    "Logging"
                                },
                                {
                                    {
                                        36,
                                        2,
                                        {
                                            "logWarn"
                                        }
                                    },
                                    {
                                        34,
                                        2,
                                        {
                                            "logError"
                                        }
                                    },
                                    {
                                        38,
                                        2,
                                        {
                                            "parseError"
                                        }
                                    },
                                    {
                                        37,
                                        2,
                                        {
                                            "messages"
                                        }
                                    },
                                    {
                                        35,
                                        2,
                                        {
                                            "logErrorNonFatal"
                                        }
                                    }
                                }
                            },
                            {
                                39,
                                2,
                                {
                                    "PubTypes"
                                }
                            },
                            {
                                48,
                                2,
                                {
                                    "Types"
                                }
                            },
                            {
                                4,
                                1,
                                {
                                    "Animation"
                                },
                                {
                                    {
                                        6,
                                        2,
                                        {
                                            "SpringScheduler"
                                        }
                                    },
                                    {
                                        9,
                                        2,
                                        {
                                            "getTweenRatio"
                                        }
                                    },
                                    {
                                        11,
                                        2,
                                        {
                                            "packType"
                                        }
                                    },
                                    {
                                        8,
                                        2,
                                        {
                                            "TweenScheduler"
                                        }
                                    },
                                    {
                                        12,
                                        2,
                                        {
                                            "springCoefficients"
                                        }
                                    },
                                    {
                                        5,
                                        2,
                                        {
                                            "Spring"
                                        }
                                    },
                                    {
                                        7,
                                        2,
                                        {
                                            "Tween"
                                        }
                                    },
                                    {
                                        13,
                                        2,
                                        {
                                            "unpackType"
                                        }
                                    },
                                    {
                                        10,
                                        2,
                                        {
                                            "lerpType"
                                        }
                                    }
                                }
                            },
                            {
                                49,
                                1,
                                {
                                    "Utility"
                                },
                                {
                                    {
                                        54,
                                        2,
                                        {
                                            "needsDestruction"
                                        }
                                    },
                                    {
                                        51,
                                        2,
                                        {
                                            "cleanup"
                                        }
                                    },
                                    {
                                        56,
                                        2,
                                        {
                                            "xtypeof"
                                        }
                                    },
                                    {
                                        50,
                                        2,
                                        {
                                            "None"
                                        }
                                    },
                                    {
                                        53,
                                        2,
                                        {
                                            "isSimilar"
                                        }
                                    },
                                    {
                                        52,
                                        2,
                                        {
                                            "doNothing"
                                        }
                                    },
                                    {
                                        55,
                                        2,
                                        {
                                            "restrictRead"
                                        }
                                    }
                                }
                            },
                            {
                                40,
                                1,
                                {
                                    "State"
                                },
                                {
                                    {
                                        44,
                                        2,
                                        {
                                            "ForValues"
                                        }
                                    },
                                    {
                                        42,
                                        2,
                                        {
                                            "ForKeys"
                                        }
                                    },
                                    {
                                        43,
                                        2,
                                        {
                                            "ForPairs"
                                        }
                                    },
                                    {
                                        45,
                                        2,
                                        {
                                            "Observer"
                                        }
                                    },
                                    {
                                        47,
                                        2,
                                        {
                                            "unwrap"
                                        }
                                    },
                                    {
                                        46,
                                        2,
                                        {
                                            "Value"
                                        }
                                    },
                                    {
                                        41,
                                        2,
                                        {
                                            "Computed"
                                        }
                                    }
                                }
                            },
                            {
                                14,
                                1,
                                {
                                    "Colour"
                                },
                                {
                                    {
                                        15,
                                        2,
                                        {
                                            "Oklab"
                                        }
                                    }
                                }
                            },
                            {
                                22,
                                1,
                                {
                                    "Instances"
                                },
                                {
                                    {
                                        31,
                                        2,
                                        {
                                            "applyInstanceProps"
                                        }
                                    },
                                    {
                                        29,
                                        2,
                                        {
                                            "Out"
                                        }
                                    },
                                    {
                                        32,
                                        2,
                                        {
                                            "defaultProps"
                                        }
                                    },
                                    {
                                        24,
                                        2,
                                        {
                                            "Cleanup"
                                        }
                                    },
                                    {
                                        27,
                                        2,
                                        {
                                            "OnChange"
                                        }
                                    },
                                    {
                                        30,
                                        2,
                                        {
                                            "Ref"
                                        }
                                    },
                                    {
                                        26,
                                        2,
                                        {
                                            "New"
                                        }
                                    },
                                    {
                                        28,
                                        2,
                                        {
                                            "OnEvent"
                                        }
                                    },
                                    {
                                        23,
                                        2,
                                        {
                                            "Children"
                                        }
                                    },
                                    {
                                        25,
                                        2,
                                        {
                                            "Hydrate"
                                        }
                                    }
                                }
                            },
                            {
                                16,
                                1,
                                {
                                    "Dependencies"
                                },
                                {
                                    {
                                        21,
                                        2,
                                        {
                                            "useDependency"
                                        }
                                    },
                                    {
                                        19,
                                        2,
                                        {
                                            "sharedState"
                                        }
                                    },
                                    {
                                        18,
                                        2,
                                        {
                                            "initDependency"
                                        }
                                    },
                                    {
                                        17,
                                        2,
                                        {
                                            "captureDependencies"
                                        }
                                    },
                                    {
                                        20,
                                        2,
                                        {
                                            "updateAll"
                                        }
                                    }
                                }
                            }
                        }
                    },
                    {
                        57,
                        2,
                        {
                            "data"
                        }
                    },
                    {
                        58,
                        2,
                        {
                            "icons"
                        }
                    },
                    {
                        61,
                        2,
                        {
                            "themeSystem"
                        },
                        {
                            {
                                62,
                                2,
                                {
                                    "default"
                                }
                            }
                        }
                    }
                }
            },
            {
                75,
                1,
                {
                    "layouts"
                },
                {
                    {
                        76,
                        2,
                        {
                            "main"
                        }
                    }
                }
            },
            {
                74,
                2,
                {
                    "gtaMenu.story"
                }
            },
            {
                77,
                2,
                {
                    "preservedConfig"
                }
            },
            {
                78,
                1,
                {
                    "simpleElements"
                },
                {
                    {
                        80,
                        2,
                        {
                            "UICorner"
                        }
                    },
                    {
                        82,
                        2,
                        {
                            "UIPadding"
                        }
                    },
                    {
                        81,
                        2,
                        {
                            "UIListLayout"
                        }
                    },
                    {
                        79,
                        2,
                        {
                            "UIAspectRatioConstraint"
                        }
                    }
                }
            }
        }
    }
}

-- Line offsets for debugging (only included when minifyTables is false)
local LineOffsets = {
    8,
    [3] = 253,
    [5] = 327,
    [6] = 545,
    [7] = 638,
    [8] = 774,
    [9] = 849,
    [10] = 892,
    [11] = 1054,
    [12] = 1153,
    [13] = 1239,
    [15] = 1328,
    [17] = 1383,
    [18] = 1440,
    [19] = 1469,
    [20] = 1493,
    [21] = 1553,
    [23] = 1583,
    [24] = 1732,
    [25] = 1753,
    [26] = 1773,
    [27] = 1809,
    [28] = 1844,
    [29] = 1881,
    [30] = 1925,
    [31] = 1955,
    [32] = 2082,
    [34] = 2193,
    [35] = 2226,
    [36] = 2261,
    [37] = 2285,
    [38] = 2331,
    [39] = 2354,
    [41] = 2501,
    [42] = 2615,
    [43] = 2864,
    [44] = 3174,
    [45] = 3421,
    [46] = 3505,
    [47] = 3568,
    [48] = 3584,
    [50] = 3731,
    [51] = 3745,
    [52] = 3799,
    [53] = 3810,
    [54] = 3828,
    [55] = 3841,
    [56] = 3870,
    [57] = 3890,
    [58] = 3962,
    [59] = 3978,
    [60] = 4802,
    [61] = 4814,
    [62] = 4883,
    [64] = 4898,
    [65] = 5110,
    [66] = 5606,
    [67] = 5958,
    [68] = 6018,
    [69] = 6255,
    [70] = 6611,
    [71] = 6758,
    [72] = 6812,
    [73] = 7139,
    [74] = 7376,
    [76] = 7559,
    [77] = 7802,
    [79] = 7813,
    [80] = 7825,
    [81] = 7833,
    [82] = 7845,
    [84] = 7880,
    [85] = 7887,
    [86] = 7906,
    [87] = 7921,
    [88] = 7934,
    [89] = 7980,
    [90] = 7987,
    [91] = 7998,
    [92] = 8011,
    [93] = 8059
}

-- Misc AOT variable imports
local WaxVersion = "0.4.1"
local EnvName = "WaxRuntime"

-- ++++++++ RUNTIME IMPL BELOW ++++++++ --

-- Localizing certain libraries and built-ins for runtime efficiency
local string, task, setmetatable, error, next, table, unpack, coroutine, script, type, require, pcall, tostring, tonumber, _VERSION =
      string, task, setmetatable, error, next, table, unpack, coroutine, script, type, require, pcall, tostring, tonumber, _VERSION

local table_insert = table.insert
local table_remove = table.remove
local table_freeze = table.freeze or function(t) return t end -- lol

local coroutine_wrap = coroutine.wrap

local string_sub = string.sub
local string_match = string.match
local string_gmatch = string.gmatch

-- The Lune runtime has its own `task` impl, but it must be imported by its builtin
-- module path, "@lune/task"
if _VERSION and string_sub(_VERSION, 1, 4) == "Lune" then
    local RequireSuccess, LuneTaskLib = pcall(require, "@lune/task")
    if RequireSuccess and LuneTaskLib then
        task = LuneTaskLib
    end
end

local task_defer = task and task.defer

-- If we're not running on the Roblox engine, we won't have a `task` global
local Defer = task_defer or function(f, ...)
    coroutine_wrap(f)(...)
end

-- ClassName "IDs"
local ClassNameIdBindings = {
    [1] = "Folder",
    [2] = "ModuleScript",
    [3] = "Script",
    [4] = "LocalScript",
    [5] = "StringValue",
}

local RefBindings = {} -- [RefId] = RealObject

local ScriptClosures = {}
local ScriptClosureRefIds = {} -- [ScriptClosure] = RefId
local StoredModuleValues = {}
local ScriptsToRun = {}

-- wax.shared __index/__newindex
local SharedEnvironment = {}

-- We're creating 'fake' instance refs soley for traversal of the DOM for require() compatibility
-- It's meant to be as lazy as possible
local RefChildren = {} -- [Ref] = {ChildrenRef, ...}

-- Implemented instance methods
local InstanceMethods = {
    GetFullName = { {}, function(self)
        local Path = self.Name
        local ObjectPointer = self.Parent

        while ObjectPointer do
            Path = ObjectPointer.Name .. "." .. Path

            -- Move up the DOM (parent will be nil at the end, and this while loop will stop)
            ObjectPointer = ObjectPointer.Parent
        end

        return Path
    end},

    GetChildren = { {}, function(self)
        local ReturnArray = {}

        for Child in next, RefChildren[self] do
            table_insert(ReturnArray, Child)
        end

        return ReturnArray
    end},

    GetDescendants = { {}, function(self)
        local ReturnArray = {}

        for Child in next, RefChildren[self] do
            table_insert(ReturnArray, Child)

            for _, Descendant in next, Child:GetDescendants() do
                table_insert(ReturnArray, Descendant)
            end
        end

        return ReturnArray
    end},

    FindFirstChild = { {"string", "boolean?"}, function(self, name, recursive)
        local Children = RefChildren[self]

        for Child in next, Children do
            if Child.Name == name then
                return Child
            end
        end

        if recursive then
            for Child in next, Children do
                -- Yeah, Roblox follows this behavior- instead of searching the entire base of a
                -- ref first, the engine uses a direct recursive call
                return Child:FindFirstChild(name, true)
            end
        end
    end},

    FindFirstAncestor = { {"string"}, function(self, name)
        local RefPointer = self.Parent
        while RefPointer do
            if RefPointer.Name == name then
                return RefPointer
            end

            RefPointer = RefPointer.Parent
        end
    end},

    -- Just to implement for traversal usage
    WaitForChild = { {"string", "number?"}, function(self, name)
        return self:FindFirstChild(name)
    end},
}

-- "Proxies" to instance methods, with err checks etc
local InstanceMethodProxies = {}
for MethodName, MethodObject in next, InstanceMethods do
    local Types = MethodObject[1]
    local Method = MethodObject[2]

    local EvaluatedTypeInfo = {}
    for ArgIndex, TypeInfo in next, Types do
        local ExpectedType, IsOptional = string_match(TypeInfo, "^([^%?]+)(%??)")
        EvaluatedTypeInfo[ArgIndex] = {ExpectedType, IsOptional}
    end

    InstanceMethodProxies[MethodName] = function(self, ...)
        if not RefChildren[self] then
            error("Expected ':' not '.' calling member function " .. MethodName, 2)
        end

        local Args = {...}
        for ArgIndex, TypeInfo in next, EvaluatedTypeInfo do
            local RealArg = Args[ArgIndex]
            local RealArgType = type(RealArg)
            local ExpectedType, IsOptional = TypeInfo[1], TypeInfo[2]

            if RealArg == nil and not IsOptional then
                error("Argument " .. RealArg .. " missing or nil", 3)
            end

            if ExpectedType ~= "any" and RealArgType ~= ExpectedType and not (RealArgType == "nil" and IsOptional) then
                error("Argument " .. ArgIndex .. " expects type \"" .. ExpectedType .. "\", got \"" .. RealArgType .. "\"", 2)
            end
        end

        return Method(self, ...)
    end
end

local function CreateRef(className, name, parent)
    -- `name` and `parent` can also be set later by the init script if they're absent

    -- Extras
    local StringValue_Value

    -- Will be set to RefChildren later aswell
    local Children = setmetatable({}, {__mode = "k"})

    -- Err funcs
    local function InvalidMember(member)
        error(member .. " is not a valid (virtual) member of " .. className .. " \"" .. name .. "\"", 3)
    end
    local function ReadOnlyProperty(property)
        error("Unable to assign (virtual) property " .. property .. ". Property is read only", 3)
    end

    local Ref = {}
    local RefMetatable = {}

    RefMetatable.__metatable = false

    RefMetatable.__index = function(_, index)
        if index == "ClassName" then -- First check "properties"
            return className
        elseif index == "Name" then
            return name
        elseif index == "Parent" then
            return parent
        elseif className == "StringValue" and index == "Value" then
            -- Supporting StringValue.Value for Rojo .txt file conv
            return StringValue_Value
        else -- Lastly, check "methods"
            local InstanceMethod = InstanceMethodProxies[index]

            if InstanceMethod then
                return InstanceMethod
            end
        end

        -- Next we'll look thru child refs
        for Child in next, Children do
            if Child.Name == index then
                return Child
            end
        end

        -- At this point, no member was found; this is the same err format as Roblox
        InvalidMember(index)
    end

    RefMetatable.__newindex = function(_, index, value)
        -- __newindex is only for props fyi
        if index == "ClassName" then
            ReadOnlyProperty(index)
        elseif index == "Name" then
            name = value
        elseif index == "Parent" then
            -- We'll just ignore the process if it's trying to set itself
            if value == Ref then
                return
            end

            if parent ~= nil then
                -- Remove this ref from the CURRENT parent
                RefChildren[parent][Ref] = nil
            end

            parent = value

            if value ~= nil then
                -- And NOW we're setting the new parent
                RefChildren[value][Ref] = true
            end
        elseif className == "StringValue" and index == "Value" then
            -- Supporting StringValue.Value for Rojo .txt file conv
            StringValue_Value = value
        else
            -- Same err as __index when no member is found
            InvalidMember(index)
        end
    end

    RefMetatable.__tostring = function()
        return name
    end

    setmetatable(Ref, RefMetatable)

    RefChildren[Ref] = Children

    if parent ~= nil then
        RefChildren[parent][Ref] = true
    end

    return Ref
end

-- Create real ref DOM from object tree
local function CreateRefFromObject(object, parent)
    local RefId = object[1]
    local ClassNameId = object[2]
    local Properties = object[3] -- Optional
    local Children = object[4] -- Optional

    local ClassName = ClassNameIdBindings[ClassNameId]

    local Name = Properties and table_remove(Properties, 1) or ClassName

    local Ref = CreateRef(ClassName, Name, parent) -- 3rd arg may be nil if this is from root
    RefBindings[RefId] = Ref

    if Properties then
        for PropertyName, PropertyValue in next, Properties do
            Ref[PropertyName] = PropertyValue
        end
    end

    if Children then
        for _, ChildObject in next, Children do
            CreateRefFromObject(ChildObject, Ref)
        end
    end

    return Ref
end

local RealObjectRoot = CreateRef("Folder", "[" .. EnvName .. "]")
for _, Object in next, ObjectTree do
    CreateRefFromObject(Object, RealObjectRoot)
end

-- Now we'll set script closure refs and check if they should be ran as a BaseScript
for RefId, Closure in next, ClosureBindings do
    local Ref = RefBindings[RefId]

    ScriptClosures[Ref] = Closure
    ScriptClosureRefIds[Ref] = RefId

    local ClassName = Ref.ClassName
    if ClassName == "LocalScript" or ClassName == "Script" then
        table_insert(ScriptsToRun, Ref)
    end
end

local function LoadScript(scriptRef)
    local ScriptClassName = scriptRef.ClassName

    -- First we'll check for a cached module value (packed into a tbl)
    local StoredModuleValue = StoredModuleValues[scriptRef]
    if StoredModuleValue and ScriptClassName == "ModuleScript" then
        return unpack(StoredModuleValue)
    end

    local Closure = ScriptClosures[scriptRef]

    local function FormatError(originalErrorMessage)
        originalErrorMessage = tostring(originalErrorMessage)

        local VirtualFullName = scriptRef:GetFullName()

        -- Check for vanilla/Roblox format
        local OriginalErrorLine, BaseErrorMessage = string_match(originalErrorMessage, "[^:]+:(%d+): (.+)")

        if not OriginalErrorLine or not LineOffsets then
            return VirtualFullName .. ":*: " .. (BaseErrorMessage or originalErrorMessage)
        end

        OriginalErrorLine = tonumber(OriginalErrorLine)

        local RefId = ScriptClosureRefIds[scriptRef]
        local LineOffset = LineOffsets[RefId]

        local RealErrorLine = OriginalErrorLine - LineOffset + 1
        if RealErrorLine < 0 then
            RealErrorLine = "?"
        end

        return VirtualFullName .. ":" .. RealErrorLine .. ": " .. BaseErrorMessage
    end

    -- If it's a BaseScript, we'll just run it directly!
    if ScriptClassName == "LocalScript" or ScriptClassName == "Script" then
        local RunSuccess, ErrorMessage = pcall(Closure)
        if not RunSuccess then
            error(FormatError(ErrorMessage), 0)
        end
    else
        local PCallReturn = {pcall(Closure)}

        local RunSuccess = table_remove(PCallReturn, 1)
        if not RunSuccess then
            local ErrorMessage = table_remove(PCallReturn, 1)
            error(FormatError(ErrorMessage), 0)
        end

        StoredModuleValues[scriptRef] = PCallReturn
        return unpack(PCallReturn)
    end
end

-- We'll assign the actual func from the top of this output for flattening user globals at runtime
-- Returns (in a tuple order): wax, script, require
function ImportGlobals(refId)
    local ScriptRef = RefBindings[refId]

    local function RealCall(f, ...)
        local PCallReturn = {pcall(f, ...)}

        local CallSuccess = table_remove(PCallReturn, 1)
        if not CallSuccess then
            error(PCallReturn[1], 3)
        end

        return unpack(PCallReturn)
    end

    -- `wax.shared` index
    local WaxShared = table_freeze(setmetatable({}, {
        __index = SharedEnvironment,
        __newindex = function(_, index, value)
            SharedEnvironment[index] = value
        end,
        __len = function()
            return #SharedEnvironment
        end,
        __iter = function()
            return next, SharedEnvironment
        end,
    }))

    local Global_wax = table_freeze({
        -- From AOT variable imports
        version = WaxVersion,
        envname = EnvName,

        shared = WaxShared,

        -- "Real" globals instead of the env set ones
        script = script,
        require = require,
    })

    local Global_script = ScriptRef

    local function Global_require(module, ...)
        local ModuleArgType = type(module)

        local ErrorNonModuleScript = "Attempted to call require with a non-ModuleScript"
        local ErrorSelfRequire = "Attempted to call require with self"

        if ModuleArgType == "table" and RefChildren[module]  then
            if module.ClassName ~= "ModuleScript" then
                error(ErrorNonModuleScript, 2)
            elseif module == ScriptRef then
                error(ErrorSelfRequire, 2)
            end

            return LoadScript(module)
        elseif ModuleArgType == "string" and string_sub(module, 1, 1) ~= "@" then
            -- The control flow on this SUCKS

            if #module == 0 then
                error("Attempted to call require with empty string", 2)
            end

            local CurrentRefPointer = ScriptRef

            if string_sub(module, 1, 1) == "/" then
                CurrentRefPointer = RealObjectRoot
            elseif string_sub(module, 1, 2) == "./" then
                module = string_sub(module, 3)
            end

            local PreviousPathMatch
            for PathMatch in string_gmatch(module, "([^/]*)/?") do
                local RealIndex = PathMatch
                if PathMatch == ".." then
                    RealIndex = "Parent"
                end

                -- Don't advance dir if it's just another "/" either
                if RealIndex ~= "" then
                    local ResultRef = CurrentRefPointer:FindFirstChild(RealIndex)
                    if not ResultRef then
                        local CurrentRefParent = CurrentRefPointer.Parent
                        if CurrentRefParent then
                            ResultRef = CurrentRefParent:FindFirstChild(RealIndex)
                        end
                    end

                    if ResultRef then
                        CurrentRefPointer = ResultRef
                    elseif PathMatch ~= PreviousPathMatch and PathMatch ~= "init" and PathMatch ~= "init.server" and PathMatch ~= "init.client" then
                        error("Virtual script path \"" .. module .. "\" not found", 2)
                    end
                end

                -- For possible checks next cycle
                PreviousPathMatch = PathMatch
            end

            if CurrentRefPointer.ClassName ~= "ModuleScript" then
                error(ErrorNonModuleScript, 2)
            elseif CurrentRefPointer == ScriptRef then
                error(ErrorSelfRequire, 2)
            end

            return LoadScript(CurrentRefPointer)
        end

        return RealCall(require, module, ...)
    end

    -- Now, return flattened globals ready for direct runtime exec
    return Global_wax, Global_script, Global_require
end

for _, ScriptRef in next, ScriptsToRun do
    Defer(LoadScript, ScriptRef)
end

-- AoT adjustment: Load init module (MainModule behavior)
return LoadScript(RealObjectRoot:GetChildren()[1])