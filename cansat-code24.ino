#define ENABLE_DEBUG_LOGGING // comment to disable logging to the serial monitor

#include <PinDefinitions.h>
#include <GlobalDefs.h>
#include <PressureTemperature.h>
#include <Accelerometer.h>
#include <GPS.h>
#include <Camera.h>
#include <RadioInterface.h>
#include <SDManagement.h>

// Create component objects
PressureTemperature pressureTemp(DebugLog);
RadioInterface radio(DebugLog);

void setup()
{
  // Open serial communication with computer
  Serial.begin(115200);

  // Configure component objects
  pressureTemp.Setup();

  // Read in a frequency from the serial port, for on the fly configuration
  Serial.print("Enter a frequency for radio communication: ");
  // TODO: set this back to 10 seconds for production
  Serial.setTimeout(1000); // 10 seconds timeout, to make sure the user has time to enter the value
  unsigned int frequency = Serial.parseInt();
  // If parseInt times out, meaning no value was entered, use the default
  if (frequency == 0)
  {
    frequency = 434000;
    Serial.println("Defaulting to 434MHz");
  }

  radio.Configure(frequency, RadioInterface::RadioPower::pwr9, RadioInterface::ParityCheck::NoCheck);
  radio.Begin();

  Serial.flush();
}

void loop()
{
  unsigned long timestamp = millis();
  float t = pressureTemp.GetTemperature(), p = pressureTemp.GetPressure();

  String message = (String) "Time = " + timestamp + "ms from startup\n"
                                                    "Temperature = " +
                   t + " ºC\n" + "Pressure = " + p + " Pa\n" + "Altitude = " + pressureTemp.GetAltitude(t, p) + " m";

  Serial.println(message);
  radio.SendLine(message, true);

  delay(timestamp + 500 - millis());
}
