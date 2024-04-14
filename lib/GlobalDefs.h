#ifndef INCL_GLOBAL_DEFS
#define INCL_GLOBAL_DEFS

#define SCHEMA_MSG F("schema[timestamp]:[pressure]:[temperature]")

typedef size_t (*PrintLog)(const char *);

inline size_t DebugLog(const char *line)
{
#ifdef ENABLE_DEBUG_LOGGING
    return Serial.println(line);
#else
    return 0;
#endif
}

#endif