#ifndef INCL_GPS
#define INCL_GPS

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

class GPS
{
private:
    SoftwareSerial _ss;
    TinyGPSPlus _gps;
    TinyGPSCustom _alt;
    unsigned long _fixes, _lastFix;

public:
    GPS();
    bool Begin();
    bool ParseData();
    void SmartDelay(unsigned long ms);
    float GetLatitude();
    float GetLongitude();
    double GetAltitude();
};

#endif