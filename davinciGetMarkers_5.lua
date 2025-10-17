-- C:\ProgramData\Blackmagic Design\DaVinci Resolve\Fusion\Scripts\Comp
-- 输出示例：Lua 5.1、Lua 5.3、LuaJIT 2.1.0-beta3 等
print(_VERSION)

local resolve = Resolve()
local project = resolve:GetProjectManager():GetCurrentProject()
local timeline = project:GetCurrentTimeline()

local endFrame = timeline:GetEndFrame() - 108000

-- 简单版本（适用于正数）
local function mathrounds(x)
    return math.floor(x + 0.5)
end

local numTable = {};
local num = 0
for timecode, marker in pairs(timeline:GetMarkers()) do
    num = timecode / endFrame * 1920
    if num then
        table.insert(numTable, mathrounds(num))
    end
end

table.sort(numTable, function(a, b)
    return a < b
end)

local strTable = {};
local index = 0
for i, value in ipairs(numTable) do
    table.insert(strTable, { index, value })
    index = value
    print(value)
end

table.sort(strTable, function(a, b)
    return a[1] < b[1]
end)

print("begin")
for key, value in ipairs(strTable) do
    print(tostring(value[1]) .. " " .. tostring(value[2]))
end

local str_01 = "<mxfile host=\"65bd71144e\">"
local str_02 = "<diagram id=\"SpSNhLwCaLvDhKGcEh6N\" name=\"第 1 页\">"
local str_03 =
"<mxGraphModel dx=\"1052\" dy=\"600\" grid=\"1\" gridSize=\"10\" guides=\"1\" tooltips=\"1\" connect=\"1\" arrows=\"1\" fold=\"1\" page=\"1\" pageScale=\"1\" pageWidth=\"1600\" pageHeight=\"900\" math=\"0\" shadow=\"0\">"
local str_04 = "<root>"
local str_05 = "<mxCell id=\"0\"/>"
local str_06 = "<mxCell id=\"1\" parent=\"0\"/>"
local str_07 =
"<mxCell id=\"2\" value=\"\" style=\"rounded=0;whiteSpace=wrap;html=1;rotation=0;fillColor=#ffe6cc;strokeColor=none;labelBackgroundColor=none;\" parent=\"1\" vertex=\"1\">"
local str_08 = "<mxGeometry width=\"1920\" height=\"40\" as=\"geometry\"/>"
local str_09 = "</mxCell>"

local str_10 = "</root>"
local str_11 = "</mxGraphModel>"
local str_12 = "</diagram>"
local str_13 = "</mxfile>"
local str_14 =
" value=\"咩咩\" style=\"text;html=1;align=center;verticalAlign=middle;whiteSpace=wrap;rounded=0;labelBackgroundColor=none;labelBorderColor=none;textShadow=0;fillColor=#d5e8d4;strokeColor=#82b366;fontFamily=阿里巴巴普惠体;fontSize=24;\" parent=\"1\" vertex=\"1\">"
local str_15 = "</mxCell>"
local str_16 =
" value=\"咩咩\" style=\"text;html=1;align=center;verticalAlign=middle;whiteSpace=wrap;rounded=0;labelBackgroundColor=none;labelBorderColor=none;textShadow=0;fillColor=#dae8fc;strokeColor=#6c8ebf;fontFamily=阿里巴巴普惠体;fontSize=24;\" vertex=\"1\" parent=\"1\">"

local file = io.open("D:\\B\xD5\xBE\\markers.dio", "w")
if file ~= nil then
    file:write(str_01 .. "\n")
    file:write(str_02 .. "\n")
    file:write(str_03 .. "\n")
    file:write(str_04 .. "\n")
    file:write(str_05 .. "\n")
    file:write(str_06 .. "\n")
    file:write(str_07 .. "\n")
    file:write(str_08 .. "\n")
    file:write(str_09 .. "\n")

    index = 3
    for key, value in pairs(strTable) do
        file:write(string.format("<mxCell id=\"%d\"", index))
        if index % 2 == 1 then
            file:write(str_14 .. "\n")
        else
            file:write(str_16 .. "\n")
        end
        file:write(string.format("<mxGeometry x=\"%d\" width=\"%d\" height=\"40\" as=\"geometry\"/>\n", value[1],
            value[2] - value[1]))
        file:write(str_15 .. "\n")
        index = index + 1
    end

    file:write(str_10)
    file:write(str_11)
    file:write(str_12)
    file:write(str_13)

    file:close()
else
    print("not open file")
end
