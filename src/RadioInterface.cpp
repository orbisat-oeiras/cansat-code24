#include <PressureTemperature.h>
#include "RadioInterface.h"

RadioInterface::RadioInterface(PrintLog pl)
    : _log(pl), _apc220(APC_TX, APC_RX)
{
    // If the SET pin is LOW, the APC is being configured, so default it to HIGH
    pinMode(APC_SET, OUTPUT);
    digitalWrite(APC_SET, HIGH);
}

void RadioInterface::Begin()
{
    // Open the SoftwareSerial port at the configured baudrate
    _apc220.begin(_configuredBaudValue);
    _log(((String) "[APC220]:[SETUP] SoftwareSerial interface opened at " + _configuredBaudValue + "bps").c_str());
}

void RadioInterface::Configure(unsigned int radioFrequency, RadioPower radioPower, ParityCheck parityCheck)
{
    /* Expected form:
    WR frequency (MHz) radio baudrate (1-4) radio power (1-9) serial baudrate (0-6) parity check (0-2) CRLF
    For some unknown reason, the APC modules don't work if the radio baudrate and serial baudrate aren't equal,
    so we set them to the same value to avoid any issues.
     */
    String command = (String) "WR " + (int)radioFrequency + ' ' + (int)_configuredBaud + ' ' + (int)radioPower + ' ' + (int)_configuredBaud + ' ' + (int)parityCheck + "\x0D\x0A";
    // SET being LOW tells the APC it's being configured
    digitalWrite(APC_SET, LOW);
    _apc220.print(command);
    // Make sure the entire string is sent before we move on
    _apc220.flush();
    digitalWrite(APC_SET, HIGH);
    _log("[APC220]:[SETUP] Succesfully configured");
}

void RadioInterface::SendLine(String line, bool flush = false)
{
    _apc220.println(line);
    if (flush)
        _apc220.flush();
}
