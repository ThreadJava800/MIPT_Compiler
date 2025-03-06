#include <cstdarg>
#include <cstdio>

#include "log.hpp"

static FILE *logfile_ptr = nullptr;

bool initLogging(const char *const logfile_name)
{
    if (logfile_name == NULL)
    {
        fprintf(stderr, "Invalid filename: '%s'!\n", logfile_name);
        return false;
    }
    
    logfile_ptr = fopen(logfile_name, "w");
    return logfile_ptr == NULL ? false : true;
}

void logPrint(const char *const fmt, ...)
{
    std::va_list args;

    if (fmt == nullptr)
    {
        fprintf(stderr, "Format string is NULL!\n");
    }

    va_start(args, fmt);
    vfprintf(logfile_ptr, fmt, args);
    va_end(args);
}

bool deinitLogging()
{
    if (logfile_ptr == nullptr)
    {
        return false;
    }

    fclose(logfile_ptr);
    logfile_ptr = nullptr;
    return true;
}
