local function test_f(para)
    print("func test: para = " .. (para or "nil"))
end

local args = ...
test_f(args)
