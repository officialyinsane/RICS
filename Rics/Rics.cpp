#include "Rics.h"
#include "Logger.h"

extern "C" {
#include "include/lua.h"
}

void Rics::Rics::init()
{
    logger.init("RICS");
    logger.info("RICS Initialised!");
}

int Rics::Rics::ping(lua_State* L)
{
    logger.info("Ping received!");
    lua_pushstring(L, "pong");
    return 1;
}

void Rics::Rics::logInfo(std::string msg) {
    logger.info(msg);
}

void Rics::Rics::logError(std::string msg) {
    logger.error(msg);
}

