#include <PressureTemperature.h>

PressureTemperature::PressureTemperature(PrintLog pl) : _sensor(&Wire, _sensor.eSDOVDD), _log(pl)
{
}

PressureTemperature::~PressureTemperature()
{
}

bool PressureTemperature::Setup(int timeoutAttempts = -1, int retryPeriod = 3000)
{
    // If timeout is disabled (timeoutAttempts is -1), set attempts to´
    // a really small number (on most Arduino's, ints are 16 bits, so
    // use that as a likely max value) so it's always smaller than -1
    int result, attempts = (timeoutAttempts == -1 ? INT16_MIN : 0);

    // Startup the sensor
    // Note: an assignment evaluates to the value being assigned, so
    // ERR_OK != (result = _sensor.begin()) will effectively call _sensor.begin(),
    // store its return value in result and compare it to ERR_OK
    while (attempts < timeoutAttempts && ERR_OK != (result = _sensor.begin()))
    {
        if (result = ERR_DATA_BUS)
            _log("[BMP388]:[SETUP] Data bus error");
        else if (result = ERR_IC_VERSION)
            _log("[BMP388]:[SETUP] Mismatched chip versions");
        _log(sprintf_get("[BMP388]:[SETUP] Retrying in {}ms", retryPeriod));
        attempts++;
    }
    if (attempts > timeoutAttempts)
        return false;

    _log("[BMP388]:[SETUP] Sensor on");

    attempts = (timeoutAttempts == -1 ? INT16_MIN : 0);
    while (attempts < timeoutAttempts && !_sensor.setSamplingMode(_sensor.eUltraPrecision))
    {
        _log(sprintf_get("[BMP388]:[SETUP] Failed top set sampling mode, retrying in {}ms", retryPeriod));
        attempts++;
    }
    if (attempts > timeoutAttempts)
    return false;

    return true;
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
