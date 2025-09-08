local timer = 0

function update(dt)
    timer = timer + dt
    if timer >= 1.0 then
        print("Hi")
        timer = 0
    end
end
