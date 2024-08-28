#ifndef INCL_ACCELEROMETER
#define INCL_ACCELEROMETER

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

class Accelerometer
{
private:
    Adafruit_ADXL345_Unified _adxl345;
    float _threshold;

public:
    // Default threshold to 10g
    Accelerometer(float threshold = 100.f);
    bool Begin();
    sensors_vec_t GetAcceleration();
    bool IsMoving();
};

#endif