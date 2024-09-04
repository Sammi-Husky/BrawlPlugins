#include <cstdio>
#include <stdarg.h>

void debug_log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}