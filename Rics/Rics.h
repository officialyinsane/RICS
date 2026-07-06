#pragma once
#pragma comment(lib, "lua.lib")

#ifdef RICS_EXPORTS
#define RICS_API __declspec(dllexport)
#else
#define RICS_API __declspec(dllimport)
#endif

extern "C" {
#include "include/lua.h"
}

namespace Rics
{
    class Rics
    {
    public:
        Rics() {}
        ~Rics() {}
    
        void init();
        
        int ping(lua_State* L);
        
        void logInfo(std::string msg);
        void logError(std::string msg);
        
        
    private:
        Logger logger;
    };
}
