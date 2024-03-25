#ifndef INCL_RADIO_INTERFACE
#define INCL_RADIO_INTERFACE
#include <GlobalDefs.h>
#include <PinDefinitions.h>
#include <SoftwareSerial.h>

/**
 * @brief Wrapper class for interfacing with the APC220 radio communication module.
 */
class RadioInterface
{
public:
    /**
     * @brief Configuration enum, represents available communication baudrates
     */
    enum class RadioBaudrate
    {
        bps2400 = 1,
        bps4800,
        bps9600,
        bps19200
    };
    /**
     * @brief Configuration enum, represents available radio power levels
     */
    enum class RadioPower
    {
        pwr0,
        pwr1,
        pwr2,
        pwr3,
        pwr4,
        pwr5,
        pwr6,
        pwr7,
        pwr8,
        pwr9
    };
    /**
     * @brief Configuration enum, represents available parity check options
     */
    enum class ParityCheck
    {
        NoCheck,
        EvenParity,
        OddParity
    };

private:
    PrintLog _log;
    /* Using HardwareSerial (pins 0 and 1) doesn't seem to work very well,
     * because it interferes with communication with the computer.
     * So, SoftwareSerial is used instead to allow any pins to be used as
     * a serial port.
     */
    SoftwareSerial _apc220;
    /* These two values hold the same information, represented differently:
       - the enum is useful to the APC because it converts directly to the expected number
       - the int is useful to Arduino because it can be passed on to SoftwareSerial
       However, this duplication os very error prone.
    */
    const RadioBaudrate _configuredBaud = RadioBaudrate::bps19200;
    const unsigned int _configuredBaudValue = 19200u;

public:
    /**
     * @brief Construct a new RadioInterface object
     *
     * @param pl A logging fuction to be used for internal messages
     */
    RadioInterface(PrintLog pl);
    /**
     * @brief Open communication channel between the Arduino and the APC220
     *
     */
    void Begin();
    /**
     * @brief Configure the APC220 module with given parameters
     *
     * @param radioFrequency Frequency for radio communications in kHz, should be in the range 420-450 MHz
     * @param radioPower Power level of the radio communications, where 9 equals 13dBm (20mW)
     * @param parityCheck Parity check mode
     */
    void Configure(unsigned int radioFrequency, RadioPower radioPower, ParityCheck parityCheck);
    /**
     * @brief Send a line of text through the APC220
     *
     * @param line The text to be sent
     * @param flush Whether to flush the Serial port before moving on
     */
    void SendLine(String line, bool flush = false);
};

#endif