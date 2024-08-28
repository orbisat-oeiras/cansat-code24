#define ENABLE_DEBUG_LOGGING // comment to disable logging to the serial monitor

#include <PinDefinitions.h>
#include <GlobalDefs.h>
#include <PressureTemperature.h>
#include <Accelerometer.h>
#include <GPS.h>
#include <Camera.h>
#include <RadioInterface.h>
#include <SDManagement.h>

/* TODO: Power saving mode
BMP388: setPWRMode
APC220: EN to a digital pin
GPS: use u-center to get the message and send it through serial
*/

// Create component objects
PressureTemperature pressureTemp;
RadioInterface radio;
SDManagement card;
Accelerometer accel(13);
GPS gps;

#define printToAll(data)     \
  Serial.print(data);        \
  radio._apc220.print(data); \
  card._file.print(data);
#define printlnToAll(data)     \
  Serial.println(data);        \
  radio._apc220.println(data); \
  card._file.println(data);

// TODO: enable buzzer!!!
bool usePowerSaving = true, powerSaving = true, buzzerEnabled = true;
unsigned long lastActiveTime = 0;
unsigned long powerSavingInactivityThreshold = 10 * 60000;

void sendSchemaMessage()
{
  printlnToAll(SCHEMA_MSG);
  radio._apc220.flush();
  card._file.sync();
}

void enablePowerSaving()
{
  pressureTemp.Standby();
  radio.Standby();
  gps.Standby();
}

void disablePowerSaving()
{
  pressureTemp.WakeUp();
  radio.WakeUp();
  gps.WakeUp();
  sendSchemaMessage();
}

void setup()
{
  // Open serial communication with computer
  Serial.begin(115200);

  // pinMode(BUZZER, OUTPUT);

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

  sendSchemaMessage();

  Serial.flush();

  pinMode(BUZZER, OUTPUT);
}

void loop()
{
  if (accel.IsMoving())
  {
    lastActiveTime = millis();
    Serial.print(F("active: "));
    Serial.println(lastActiveTime);
  }

  if (usePowerSaving)
  {
    if (powerSaving)
    {
      if (millis() <= lastActiveTime + powerSavingInactivityThreshold)
      {
        Serial.println("Exiting power saving mode");
        powerSaving = false;
        disablePowerSaving();
      }
      else
      {
        if (buzzerEnabled)
        {
          Serial.println("Buzz");
          digitalWrite(BUZZER, HIGH);
          delay(250);
          digitalWrite(BUZZER, LOW);
          delay(3000);
        }
        return;
      }
    }
    else if (millis() >= lastActiveTime + powerSavingInactivityThreshold)
    {
      Serial.println("Entering power saving mode");
      powerSaving = true;
      enablePowerSaving();
      return;
    }
  }

  unsigned long timestamp = millis();
  float t = pressureTemp.GetTemperature(), p = pressureTemp.GetPressure();
  sensors_vec_t acceleration = accel.GetAcceleration();
  float lat = gps.GetLatitude(), lon = gps.GetLongitude(), alt = gps.GetAltitude();

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

  // digitalWrite(BUZZER, HIGH);
  gps.SmartDelay(timestamp + 500 - millis());
  // digitalWrite(BUZZER, LOW);
}

/*
loop:
  se (o cansat está a subir):
    ligado = true;
    pin FakeVCC = HIGH;
    registarTimer(400, lerPressãoTemperatura);
    processVídeo();


lerPressãoTemperatura:
  pressão, temperatura = BMP388.ler();
  coord = gps.ler();
  antena1.escrever(pressão, temperatura, coord, millis());
  sd.escrever(pressão, temperatura, coord, millis());

prepararVídeo:
  sd.escreve("vídeo iniciado a " + millis());
  releCameraAntena.fecha();
  camera.iniciaVídeo();
*/