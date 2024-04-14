#include "Accelerometer.h"

Accelerometer::Accelerometer(float threshold) : _adxl345(0), _threshold(threshold)
{
}

bool Accelerometer::Begin()
{
    if (!_adxl345.begin())
    {
        Serial.println(F("[ADXL345]:[SETUP] Failed to initialize sensor"));
        return false;
    }

    _adxl345.setRange(ADXL345_RANGE_16_G);
    Serial.println(F("[ADXL345]:[SETUP] Sensor configured with range +/-16g."));
}

sensors_vec_t Accelerometer::GetAcceleration()
{
    sensors_event_t evt;
    _adxl345.getEvent(&evt);
    return evt.acceleration;
}

bool Accelerometer::IsMoving()
{
    sensors_vec_t acl = GetAcceleration();
    return max(max(acl.x, acl.y), acl.z) >= _threshold;
}
