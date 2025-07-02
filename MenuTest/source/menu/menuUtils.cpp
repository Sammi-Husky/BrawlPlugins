#include "menu/menuUtils.h"

#include <memory.h>

void Printf(nw4r::ut::TextWriterBase<char>& writer, const char* format, ...)
{
    va_list args;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    writer.Print(buffer, len);
}