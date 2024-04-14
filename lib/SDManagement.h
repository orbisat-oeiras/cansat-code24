#ifndef INCL_SD_MANAGEMENT
#define INCL_SD_MANAGEMENT

#include <SD.h>
#include <GlobalDefs.h>

class SDManagement
{
private:
    Sd2Card _card;
    SdVolume _volume;

public:
    SdFile _file;
    SDManagement();
    ~SDManagement();
    bool Setup(const char *filename = "log.txt");
};

#endif