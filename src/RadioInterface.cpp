#include <PressureTemperature.h>
#include "RadioInterface.h"

RadioInterface::RadioInterface()
    : _apc220(APC_TX, APC_RX)
{
    // If the SET pin is LOW, the APC is being configured, so default it to HIGH
    pinMode(APC_SET, OUTPUT);
    digitalWrite(APC_SET, HIGH);
}

void RadioInterface::Begin()
{
    // Open the SoftwareSerial port at the configured baudrate
    _apc220.begin(_configuredBaudValue);
    //_apc220.println(SCHEMA_MSG);
    //_apc220.flush();
    Serial.print(F("[APC220]:[SETUP] SoftwareSerial interface opened at "));
    Serial.print(_configuredBaudValue);
    Serial.println(F("bps"));
}

void RadioInterface::Configure(unsigned int radioFrequency, RadioPower radioPower, ParityCheck parityCheck)
{
    /* Expected form:
    WR frequency (MHz) radio baudrate (1-4) radio power (1-9) serial baudrate (0-6) parity check (0-2) CRLF
    For some unknown reason, the APC modules don't work if the radio baudrate and serial baudrate aren't equal,
    so we set them to the same value to avoid any issues.
     */
    // SET being LOW tells the APC it's being configured
    digitalWrite(APC_SET, LOW);
    _apc220.print(F("WR "));
    _apc220.print(radioFrequency);
    _apc220.print(' ');
    _apc220.print((int)_configuredBaud);
    _apc220.print(' ');
    _apc220.print((int)radioPower);
    _apc220.print(' ');
    _apc220.print((int)_configuredBaud);
    _apc220.print(' ');
    _apc220.print((int)parityCheck);
    _apc220.print(F("\x0D\x0A"));
    // Make sure the entire string is sent before we move on
    _apc220.flush();
    digitalWrite(APC_SET, HIGH);
    Serial.println(F("[APC220]:[SETUP] Succesfully configured"));
}
