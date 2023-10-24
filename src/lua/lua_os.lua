--[[
-- Lua-OS: Writed by Lua, execute lua
--]]

-- ls
local function ls(para)
    local i
    for i = 1, #ramfs do
        print(ramfs[i][1])
    end
end

-- cat
local function cat(file)
    local i
    for i = 1, #ramfs do
        if file == ramfs[i][1] then
            print(ramfs[i][2])
            return
        end
    end
    print("No file: " .. file)
end

-- help
local function help()
    print("ls           : List the files")
    print("cat <file>   : Show the file content")
    print("<file>       : Execute the lua script file")
    print("<script>     : Execute the lua script from stdin")
    print("help | ?     : List this help information")
end

local commands = {
    {"ls",   ls},
    {"cat",  cat},
    {"help", help},
    {"?" ,   help},
}

-- Lua-OS shell
local function shell(str)
    -- the return only, do nothing
    if (#str == 0) then
        return
    end

    -- separate the command and arguments
    local s, e = string.find(str, " ")
    local cmd = str
    local largs = ""
    if (s) then
        cmd = string.sub(str, 1, s - 1)
        largs = string.sub(str, s + 1)
    end

    -- commands
    local i
    for i = 1, #commands do
        if commands[i][1] == cmd then
            commands[i][2](largs)
            return
        end
    end

    -- file
    for i = 1, #ramfs do
        if ramfs[i][1] == cmd then
            local func = load(ramfs[i][2])
            local status, err = pcall(func, largs)
            return
        end
    end

    -- script
    local func = load(str)
    local status, err = pcall(func)
    if not status then
        print(err)
    end
    return
end

-- Lua OS entry
-- find the LOGO.txt in ramfs
local i, logo
for i = 1, #ramfs do
    if ramfs[i][1] == "LOGO.txt" then
        logo = ramfs[i][2]
        break
    end
end

print(logo)

while true do
    io.write ("Lua-OS # ")
    io.flush()
    s = io.read("*l")

    if (s == nil) then
        print("nil")
        break
    end

    shell(s)
end
