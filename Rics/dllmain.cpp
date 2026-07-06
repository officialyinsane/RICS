#pragma comment(lib, "lua.lib")

#include <iostream>
#include <ostream>
#include <string>
#include "include/spdlog/spdlog.h"
#include "include/spdlog/sinks/basic_file_sink.h"
#include "Rics.h"

extern "C" {
#include "include/lua.h"
#include "include/lauxlib.h"
}

Rics::Rics g_Rics;

static int ping(lua_State *L) {
	return g_Rics.ping(L);
}


extern "C" int __declspec(dllexport) luaopen_Rics(lua_State *L) {
	try
	{
		g_Rics.init();
		
		g_Rics.logInfo("Rics DLL Loading!");

		static const luaL_Reg Map[] = {
			{ "ping", ping },		/* string	Rics.ping() */
			{ NULL,NULL }
		};
		
		luaL_register(L, "Rics", Map);
		g_Rics.logInfo("Rics Functions Loaded!");
		return 1;
	} catch (const spdlog::spdlog_ex &ex) {
		g_Rics.logError("Log init failed! " + std::string(ex.what()));
	}
	return 1;
	
}