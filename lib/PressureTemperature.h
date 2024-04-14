#ifndef INCL_PRESSURE_TEMPERATURE
#define INCL_PRESSURE_TEMPERATURE

#include <DFRobot_BMP3XX.h>
#include <GlobalDefs.h>

/**
 * @brief Wrapper class for interfacing with the BMP388 atmospheric pressure and temperature sensor.
 */
class PressureTemperature
{
private:
    DFRobot_BMP388_I2C _sensor;

public:
    /**
     * @brief Construct a new PressureTemperature object
     *
     * @param pl A logging fuction to be used for internal messages
     */
    PressureTemperature();
    /**
     * @brief Destroy the PressureTemperature object
     * @todo unnecessary destructor, it seems
     */
    ~PressureTemperature();

    /**
     * @brief Prepares the PressureTemperature object for operation
     *
     * @param timeoutAttempts How many attempts to initalise the sensor before failure. -1 for infinite attempts.
     * @param retryPeriod How long to wait between attempts to initialise the sensor, in milliseconds
     * @return true if the sensor was successfully initialised;
     * @return false otherwise
     */
    bool Setup(int timeoutAttempts = -1, int retryPeriod = 3000);

    /**
     * @brief Get the temperature read by the sensor
     *
     * @return [float] Temperature in ºC
     */
    float GetTemperature();
    /**
     * @brief Get the pressure read by the sensor
     *
     * @return [float] Pressure in Pa
     */
    float GetPressure();
    /**
     * @brief Get the altitude computed from the temperature and pressure values
     *
     * @param temperature in ºC
     * @param pressure in Pa
     * @return [float] Computed altitude in meters
     */
    float GetAltitude(float temperature, float pressure);
};

#endif