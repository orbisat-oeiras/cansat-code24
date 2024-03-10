#include <GlobalDefs.h>


template <typename... Args>
const char *sprintf_get(const char *format, Args... args)
{
    char *result;
    sprintf(result, format, args...);
    return result;
}