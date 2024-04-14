#ifndef INCL_GPS
#define INCL_GPS

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

class GPS
{
private:
    SoftwareSerial _ss;
    TinyGPSPlus _gps;

public:
    GPS();
    bool Begin();
    bool ParseData();
    float GetLatitude();
    float GetLongitude();
    float GetAltitude();
};

#endif