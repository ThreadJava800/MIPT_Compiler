#pragma once

extern bool initLogging(const char *const logfile_name);
extern void logPrint(const char *const fmt, ...);
extern bool deinitLogging();

#define DBG_ERR(fmt, ...) \
    logPrint("Compiler error in ( %s : %d)" fmt, __FILE__, __LINE__, ##__VA_ARGS__);
