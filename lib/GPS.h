#ifndef INCL_GPS
#define INCL_GPS

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#define GPS_SLEEP F("\x00\x00  \xB5 \x62 \x02 \x41 \x08 \x00 \x00 \x00 \x00 \x00 \x02 \x00 \x00 \x00 \x4D \x3B")

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

    void Standby();
    void WakeUp();
};

#endif