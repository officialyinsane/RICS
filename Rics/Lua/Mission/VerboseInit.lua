package.cpath = package.cpath..";"..lfs.writedir().."Mods\\Services\\Rics\\bin\\?.dll;"

Rics = {}
function Rics.log(_level, _str)
    log.write("Rics", _level, _str);
end

function Rics.info(_str)
    Rics.log(log.INFO, _str);
end

function Rics.error(_str)
    Rics.log(log.ERROR, _str);
end

local Rics = nil;
local _status, _result = pcall(function()
    Rics = require("Rics")
end);

if _status then
    Rics.info("Loading completed successfully.")
else
    Rics.error("Failed to load with result: " .. _result);
end

if Rics then
    local _response = Rics.ping();
    Rics.info("Response from the DLL was: " .. _response);
else
    Rics.error("Unable to ping");
end 