--[[
-- Lua-OS: Writed by Lua, execute lua
-- The ramfs is built in by build system and attach to this Lua context
--]]

-- ls
local function ls(para)
    for n, _ in pairs(ramfs) do
        print(n)
    end
end

-- cat
local function cat(file)
    local f = ramfs[file]
    print(f or "No file: " .. file)
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
    ["ls"]   = ls,
    ["cat"]  = cat,
    ["?"]    = help,
    ["help"] = help,
}

-- execute the Lua script
local function execute(script, largs)
    local func = load(script)
    local status, err = pcall(func, largs)
    if not status then
        print(err)
    end
end

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
    local c = commands[cmd]
    if c then
        return c(largs)
    end

    -- file
    local f = ramfs[cmd]
    if f then
        return execute(f, largs)
    end

    -- script from stdin
    return execute(str)
end

-- Lua-OS entry
-- Show the LOGO.txt in ramfs
print(ramfs["LOGO.txt"] or "")

while true do
    io.write ("Lua-OS # ")
    io.flush()
    s = io.read("*l")

    if (s == nil) then
        break
    end

    shell(s)
end
