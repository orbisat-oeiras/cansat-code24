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
PressureTemperature pressureTemp;
RadioInterface radio;
SDManagement card;

void setup()
{
  // Open serial communication with computer
  Serial.begin(115200);

  // Configure component objects
  pressureTemp.Setup();

  // Read in a frequency from the serial port, for on the fly configuration
  Serial.print(F("Enter a frequency for radio communication: "));
  // TODO: set this back to 10 seconds for production
  Serial.setTimeout(1000); // 10 seconds timeout, to make sure the user has time to enter the value
  unsigned int frequency = Serial.parseInt();
  // If parseInt times out, meaning no value was entered, use the default
  if (frequency == 0)
  {
    frequency = 433250;
    Serial.println(F("Defaulting to organization-attributed 433250Hz"));
  }

  radio.Configure(frequency, RadioInterface::RadioPower::pwr9, RadioInterface::ParityCheck::NoCheck);
  radio.Begin();
  card.Setup();

  radio._apc220.println(SCHEMA_MSG);
  radio._apc220.flush();
  card._file.println(SCHEMA_MSG);
  card._file.sync();

  Serial.flush();
}

void loop()
{
  unsigned long timestamp = millis();
  float t = pressureTemp.GetTemperature(), p = pressureTemp.GetPressure();

  Serial.print(F("["));
  Serial.print(timestamp);
  Serial.print(F("]:["));
  Serial.print(p);
  Serial.print(F("]:["));
  Serial.print(t);
  Serial.println(F("];"));
  Serial.flush();

  radio._apc220.print(F("["));
  radio._apc220.print(timestamp);
  radio._apc220.print(F("]:["));
  radio._apc220.print(p);
  radio._apc220.print(F("]:["));
  radio._apc220.print(t);
  radio._apc220.println(F("];"));
  radio._apc220.flush();

  card._file.print(F("["));
  card._file.print(timestamp);
  card._file.print(F("]:["));
  card._file.print(p);
  card._file.print(F("]:["));
  card._file.print(t);
  card._file.println(F("];"));
  card._file.sync();

  delay(timestamp + 500 - millis());
}
