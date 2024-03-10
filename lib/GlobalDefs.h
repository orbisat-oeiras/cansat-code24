#ifndef INCL_GLOBAL_DEFS
#define INCL_GLOBAL_DEFS

typedef size_t (*PrintLog)(const char *);

size_t Serial_println(const char *line);

template <typename... Args>
const char *sprintf_get(const char *format, Args... args);

#endif