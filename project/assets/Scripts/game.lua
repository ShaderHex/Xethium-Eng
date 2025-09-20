-- =================================================================
-- Template Lua Script for Xethium Engine
-- =================================================================

-- Global variables
local timer = 0
local CubePos = 0
local CubeColor = 0

-- Called once when the script starts
function Start()
    --print("Game started!")
end

-- Called every frame; dt = delta time (seconds)
function Update(dt)
    timer = timer + dt

    -- Move cube every 0.1 seconds for testing
    if timer >= 0.1 then
        -- Move Cube0 up along Y axis
        MoveRectangleByName("Cube0", 0, CubePos, 0)

        -- Change cube color gradually
        SetColorByName("Cube0", CubeColor % 256, CubeColor % 256, 255, 255)

        -- Increment counters
        CubePos = CubePos + 0.1 
        CubeColor = CubeColor + 5

        print("Cube0 moved to Y=" .. CubePos .. " | Color=" .. CubeColor % 256)

        timer = 0
    end
end