#include "Logger.h"

#include "include/spdlog/spdlog.h"
#include "include/spdlog/sinks/basic_file_sink.h"

void Logger::init(std::string module) {
    loggerName = module;
    auto existing_logger = spdlog::get(module);
    if (!existing_logger) {
        spdlog::basic_logger_mt(module, "c:\\temp\\risc.log");
    }
    spdlog::flush_on(spdlog::level::debug);
}

void Logger::trace(std::string msg) {
    auto l = spdlog::get(loggerName);
    if (l) l->trace(msg);
}

void Logger::debug(std::string msg) {
    auto l = spdlog::get(loggerName);
    if (l) l->debug(msg);
}

void Logger::info(std::string msg) {
    auto l = spdlog::get(loggerName);
    if (l) l->info(msg);
}

void Logger::warn(std::string msg) {
    auto l = spdlog::get(loggerName);
    if (l) l->warn(msg);
}

void Logger::error(std::string msg) {
    auto l = spdlog::get(loggerName);
    if (l) l->error(msg);
}
