#ifndef INCL_GLOBAL_DEFS
#define INCL_GLOBAL_DEFS

typedef size_t (*PrintLog)(const char *);

inline size_t Serial_println(const char *line)
{
    return Serial.println(line);
}

#endif