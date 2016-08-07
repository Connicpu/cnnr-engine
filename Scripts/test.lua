local v1 = math.Vec2(1, 2)
local v2 = math.Vec2(3, 4)
print(2*v1 + v2) -- <5.00000, 8.00000>

v1.x = v1.x + 1
print(v1:topoint()) -- (2.00000, 2.00000)

