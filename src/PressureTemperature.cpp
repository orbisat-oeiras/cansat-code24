#include <PressureTemperature.h>

PressureTemperature::PressureTemperature() : _sensor(&Wire, _sensor.eSDOVDD)
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
