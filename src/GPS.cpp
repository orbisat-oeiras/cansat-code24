#include "GPS.h"
#include <PinDefinitions.h>

GPS::GPS() : _ss(GPS_TX, GPS_RX)
{
}

bool GPS::Begin()
{
    _ss.begin(9600);
    Serial.println(F("[GPS]:[SETUP] Opened serial communication at 9600bps"));
    return true;
}

bool GPS::ParseData()
{
    while (_ss.available() > 0)
    {
        if (_gps.encode(_ss.read()))
            return true;
    }
    return false;
}

float GPS::GetLatitude()
{
    if (!_gps.location.isValid())
        return NAN;
    return _gps.location.lat();
}

float GPS::GetLongitude()
{
    if (!_gps.location.isValid())
        return NAN;
    return _gps.location.lng();
}

float GPS::GetAltitude()
{
    if (!_gps.altitude.isValid())
        return NAN;
    return _gps.altitude.meters();
}
