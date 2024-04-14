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
Accelerometer accel(15);
GPS gps;

bool usePowerSaving = true, powerSaving = true;
int powerSavingStartTime = 0, powerSavingOffDuration = 5 * 60000;

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

  accel.Begin();

  gps.Begin();

  radio._apc220.println(SCHEMA_MSG);
  radio._apc220.flush();
  card._file.println(SCHEMA_MSG);
  card._file.sync();

  Serial.flush();
}

void loop()
{
  if (usePowerSaving && powerSaving)
  {
    if (accel.IsMoving())
    {
      Serial.println("Exiting power saving mode");
      powerSaving = false;
      powerSavingStartTime = millis();
      // TODO: close a transistor so other components are on
    }
    else
    {
      return;
    }
  }

  if (millis() >= powerSavingStartTime + powerSavingOffDuration)
  {
    powerSaving = true;
  }

  unsigned long timestamp = millis();
  float t = pressureTemp.GetTemperature(), p = pressureTemp.GetPressure();
  sensors_vec_t acceleration = accel.GetAcceleration();
  gps.ParseData();
  float lat = gps.GetLatitude(), lon = gps.GetLongitude(), alt = gps.GetAltitude();

#define printToAll(data)     \
  Serial.print(data);        \
  radio._apc220.print(data); \
  card._file.print(data);
#define printlnToAll(data)     \
  Serial.println(data);        \
  radio._apc220.println(data); \
  card._file.println(data);

  printToAll(F("["));
  printToAll(timestamp);
  printToAll(F("]:["));
  printToAll(p);
  printToAll(F("]:["));
  printToAll(t);
  printToAll(F("]:["));
  printToAll(acceleration.x);
  printToAll(F("]:["));
  printToAll(acceleration.y);
  printToAll(F("]:["));
  printToAll(acceleration.z);
  printToAll(F("]:["));
  printToAll(lat);
  printToAll(F("]:["));
  printToAll(lon);
  printToAll(F("]:["));
  printToAll(alt);
  printlnToAll(F("];"));
  Serial.flush();
  radio._apc220.flush();
  card._file.sync();

  delay(timestamp + 500 - millis());
}
