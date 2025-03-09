#pragma once

extern bool initLogging(const char *const logfile_name);
extern void logPrint(const char *const fmt, ...);
extern bool deinitLogging();

#define DEV_DBG_ERR(fmt, ...) \
    logPrint("Compiler error in (%s : %d): " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

#define DEV_ASSERT(case_)           \
    if ((case_))                    \
    {                               \
        DEV_DBG_ERR("%s\n", #case_) \
        deinitLogging();            \
        abort();                    \
    }

#define USER_ERR(fmt, ...) \
    logPrint("Error: " fmt, ##__VA_ARGS__);
