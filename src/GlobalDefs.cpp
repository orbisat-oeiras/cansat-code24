#include <GlobalDefs.h>

size_t Serial_println(const char *line)
{
    return Serial.println(line);
}

template <typename... Args>
const char *sprintf_get(const char *format, Args... args)
{
    char *result;
    sprintf(result, format, args...);
    return result;
}