#include "GPS.h"
#include <PinDefinitions.h>

GPS::GPS() : _ss(GPS_TX, GPS_RX), _alt(_gps, "$GPGGA", 10)
{
}

bool GPS::Begin()
{
    _ss.begin(9600);
    Serial.println(F("[GPS]:[SETUP] Opened serial communication at 9600bps"));
    return true;
}

// This custom version of delay() ensures that the gps object
// is being "fed".
void GPS::SmartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (_ss.available())
            _gps.encode(_ss.read());

        if (_gps.sentencesWithFix() > _fixes)
        {
            _fixes = _gps.sentencesWithFix();
            Serial.println(millis() - _lastFix);
            _lastFix = millis();
        }
    } while (millis() - start < ms);
}

bool GPS::ParseData()
{
    while (_ss.available() > 0)
    {
        char c = _ss.read();
        // Serial.print(c);
        if (_gps.encode(c))
            return true;
    }
    if (_gps.sentencesWithFix() > _fixes)
    {
        _fixes = _gps.sentencesWithFix();
        Serial.println(millis() - _lastFix);
        _lastFix = millis();
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

double GPS::GetAltitude()
{
    /*if (!_alt.isValid())
        return "";
    return _alt.value();*/
    return _gps.altitude.meters();
}

void GPS::Standby()
{
    if (_ss.availableForWrite())
    {
        _ss.write(GPS_SLEEP);
    }
}

void GPS::WakeUp()
{
    if (_ss.availableForWrite())
    {
        _ss.write("0");
    }
}
