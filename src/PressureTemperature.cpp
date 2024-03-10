#include <PressureTemperature.h>

PressureTemperature::PressureTemperature(PrintLog pl) : _sensor(&Wire, _sensor.eSDOVDD), _log(pl)
{
}

PressureTemperature::~PressureTemperature()
{
}

bool PressureTemperature::Setup(int timeout, int retryPeriod)
{
    return false;
}

float PressureTemperature::GetTemperature()
{
    return 0.0f;
}

float PressureTemperature::GetPressure()
{
    return 0.0f;
}

float PressureTemperature::GetAltitude(float temperature, float pressure)
{
    return 0.0f;
}
