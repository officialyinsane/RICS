# Rics

## Introduction

RICS is the **Runtime Integration Control System**.

More accurately, this project is a minimal example of how to load a native DLL into DCS and expose C/C++ functions to DCS Lua.

This is intended as a starting point for experimentation, cockpit integration, state synchronisation, external tooling, and other DCS Lua/native interop work.

A picture says a thousand words, apparently, so here's my scribble:
                  DCS
                   │
      ┌────────────┴────────────┐
      │                         │
 Hooks Lua                 Mission Lua
      │                         │
      └────────────┬────────────┘
                   │
                Rics.dll
                   │
            Native C/C++ Code

RICS is a stripped-down version of something I'm slowly building over time. The rest is currently secret squirrel tech.

## Warning

This project requires changes to DCS Lua sandboxing, specifically `MissionScripting.lua`.

That means you are deliberately relaxing some of DCS's default protections. Only use DLLs and Lua scripts you trust, and expect to repeat the change after DCS updates.

## Quick Start

1. Build the project.
2. Copy the built DLL from `x64/Release`.
3. Ensure the DLL is named `Rics.dll`.
4. Place it here, creating directories as needed:
   ```text
   Saved Games\DCS\Mods\Services\Rics\bin\
   ```
5. Add the Lua hook file to your DCS Hooks directory:
   ```text
   Saved Games\DCS\Scripts\Hooks\
   ```
6. Start DCS.
7. Modify the Lua/C++ examples and rebuild as needed.

## A Note on the DCS Lua Environments

DCS exposes multiple independent Lua environments, each with different capabilities. They are intentionally sandboxed and cannot directly access one another.

For example, the Mission environment cannot access the player's cockpit state. (Trust me, I once tried to detect players taking off from a taxiway and punish them by pulling their ejection handle. Sadly, Eagle Dynamics thought better of that.)

If you want to interact with the player's cockpit, controls, or aircraft state, you'll need to load your Lua from the **Hooks** environment. See [VerboseHook.lua](./Rics/Lua/Hooks/VerboseHook.lua) for an example.

If you want to interact with the wider simulation—such as the `world`, airfields, warehouses, units, triggers, or other mission objects - you'll need to execute Lua in the **Mission** environment. See [VerboseInit.lua](./Rics/Lua/Mission/VerboseInit.lua) for an example of adding this through the Mission Editor.

This DLL can be loaded into either environment, allowing you to expose native functionality wherever it is needed. However, I haven't tried to load the DLL into both environments at the same time. It may work, it may not. I would guess you get two copies of the DLL that can't actually share the same memory space. Here be dragons.

You may also wish to make use of the various callback functions that DCS invokes during simulation. These allow your Lua code to execute periodically or once per simulation frame. Explaining them is outside the scope of this project, but you'll probably want to look at functions such as `LuaExportBeforeNextFrame()`, `LuaExportAfterNextFrame()`, and `LuaExportActivityNextEvent()`. Consider this a gentle nudge in the right direction.

## Loading the DLL from Lua

The minimum Lua required to load the DLL is:

```lua
package.cpath = package.cpath .. ";" .. lfs.writedir() .. "Mods\\Services\\Rics\\bin\\?.dll;"
Rics = require("Rics")
```

If you move the DLL to a different directory, update the `package.cpath` entry.

If you rename the DLL, update the `require()` call and the native Lua open function. See [DLL Naming](#dll-naming).

## MissionScripting.lua

By default, DCS sanitises parts of the Lua environment. In particular, `package` is disabled, which prevents loading native DLLs through `require()`.

To allow this project to work, `MissionScripting.lua` must be adjusted so that `package.cpath` remains available before sanitisation removes access to `package`.

DCS updates may overwrite this file, so you may need to repeat the change after every update.

## DLL Naming

DCS/Lua expects the native module open function to match the DLL filename.

For a DLL named:
```text
Rics.dll
```
the exported open function must be:
```cpp
luaopen_Rics
```

If you rename the DLL, you must also rename the corresponding `luaopen_<module>` function and update the Lua `require()` call.

For example:
```lua
MyModule = require("MyModule")
```
expects:
```cpp
luaopen_MyModule
```

## Adding New Lua-callable Methods

Most of the useful native binding code is in [dllmain.cpp](./Rics/dllmain.cpp).

To expose a new function to Lua, add it to the function map.

See the existing `ping` method for the simplest example.

## Supported Methods

| Method | Parameters | Description                          |
| ------ | ---------- | ------------------------------------ |
| `ping` | None       | Returns the constant string `"pong"` |

## Gotchas

1. DCS currently requires a specific Lua version. This project includes the required Lua dependency, but Eagle Dynamics may change this in future.
2. Lua in DCS is single-threaded. Avoid long-running work inside Lua unless you enjoy murdering your frame rate.
3. Keep expensive native work off the simulation thread where possible.
4. After every DCS update, check whether your `MissionScripting.lua` changes still exist (spoiler, they won't).
5. If the DLL loads but `require("Rics")` fails, check the DLL filename, exported `luaopen_Rics` function, and `package.cpath`.

## Credits

* Ciribob, for [DCS SRS](https://github.com/ciribob/DCS-SimpleRadioStandalone). Without that project being open source, this would have involved a lot more digging through forum archaeology.
* OzDeadMeat, for the nagging that made me actually figure out how to load a DLL into DCS.
* Goose1982, for [PassthroughForge](https://www.passthroughforge.com/). Great software, and the Discord that led to this rabbit hole.
* Loonz_LB, for asking the question that made me realise I had already solved part of the problem. Apologies for adding to your rabbit holes.
