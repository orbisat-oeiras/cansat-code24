#include <PressureTemperature.h>

PressureTemperature::PressureTemperature() : _sensor(&Wire, _sensor.eSDOVDD)
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
        if (result == ERR_DATA_BUS)
            Serial.println(F("[BMP388]:[SETUP] Data bus error"));
        else if (result == ERR_IC_VERSION)
            Serial.println(F("[BMP388]:[SETUP] Mismatched chip versions"));
        Serial.print(F("[BMP388]:[SETUP] Retrying in "));
        Serial.print(retryPeriod);
        Serial.println(F("ms"));
        attempts++;
    }
    if (attempts > timeoutAttempts)
        return false;

    Serial.println(F("[BMP388]:[SETUP] Sensor on"));

    attempts = (timeoutAttempts == -1 ? INT16_MIN : 0);
    while (attempts < timeoutAttempts && !_sensor.setSamplingMode(_sensor.eUltraPrecision))
    {
        Serial.print(F("[BMP388]:[SETUP] Failed top set sampling mode, retrying in "));
        Serial.print(retryPeriod);
        Serial.println(F("ms"));
        attempts++;
    }
    Serial.println(F("[BMP388]:[SETUP] Sampling mode configured"));
    if (attempts > timeoutAttempts)
        return false;

    Serial.print(F("[BMP388]:[SETUP] Sampling period = "));
    Serial.print(_sensor.getSamplingPeriodUS());
    Serial.print(F("us, sampling frequency = "));
    Serial.print(1E6 / _sensor.getSamplingPeriodUS());
    Serial.println(F("Hz"));
    Serial.flush();

    return true;
}

float PressureTemperature::GetTemperature()
{
    return _sensor.readTempC();
}

float PressureTemperature::GetPressure()
{
    return _sensor.readPressPa();
}

float PressureTemperature::GetAltitude(float temperature, float pressure)
{
    // See https://physics.stackexchange.com/questions/333475/how-to-calculate-altitude-from-current-temperature-and-pressure
    // for more details

    // Define constants
    const float p0 = 101325.f, exp = 1 / 5.257f, abs0 = -273.15, L = 0.0065f;
    // Compute the altitude
    float pTerm = powf(p0 / pressure, exp) - 1;
    float tTerm = temperature - abs0;
    return pTerm * tTerm / L;
}

void PressureTemperature::Standby()
{
    _sensor.setPWRMode(_sensor.eSleepMode);
}

void PressureTemperature::WakeUp()
{
    _sensor.setPWRMode(_sensor.eNormalMode);
}
