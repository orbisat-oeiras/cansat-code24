#include <PinDefinitions.h>
#include <GlobalDefs.h>
#include <PressureTemperature.h>
#include <Accelerometer.h>
#include <GPS.h>
#include <Camera.h>
#include <RadioCommunication.h>
#include <SDManagement.h>

PressureTemperature pressureTemp(Serial_println);

void setup()
{
  Serial.begin(115200);
  pressureTemp.Setup();
  Serial.flush();
}

void loop()
{
  float t = pressureTemp.GetTemperature(), p = pressureTemp.GetPressure();
  Serial.println((String)"Temperature = "+t+" ºC");
  Serial.println((String)"Pressure = "+p+" Pa");
  Serial.println((String)"Altitude = "+pressureTemp.GetAltitude(t, p)+" m");
  delay(5000);
}
