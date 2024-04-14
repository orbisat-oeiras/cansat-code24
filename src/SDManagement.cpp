#include <SDManagement.h>
#include <PinDefinitions.h>

SDManagement::SDManagement()
{
}

SDManagement::~SDManagement()
{
    _file.close();
}

bool SDManagement::Setup(const char *filename = "log.txt")
{
    // Set the Arduino's SS to output, otherwise the SD library won't work
    pinMode(ARD_SS, OUTPUT);
    if (!_card.init(SPI_HALF_SPEED, SD_SS))
    {
        Serial.println(F("[SD]:[SETUP] Initialization failed."));
        return false;
    }

    if (_volume.init(_card) == 0)
    {
        Serial.println(F("[SD]:[SETUP] Unable to find valid volume."));
        return false;
    }

    // Print info
    // print the type of card
    Serial.print(F("[SD]:[SETUP] Card type: "));
    switch (_card.type())
    {
    case SD_CARD_TYPE_SD1:
        Serial.println(F("SD1"));
        break;
    case SD_CARD_TYPE_SD2:
        Serial.println(F("SD2"));
        break;
    case SD_CARD_TYPE_SDHC:
        Serial.println(F("SDHC"));
        break;
    default:
        Serial.println(F("Unknown"));
    }
    Serial.print(F("[SD]:[SETUP] Volume type is FAT"));
    Serial.println(_volume.fatType(), DEC);

    SdFile root;

    bool result = root.openRoot(_volume);
    if (!result)
    {
        Serial.println(F("[SD]:[SETUP] Failed to open volume root"));
        return false;
    }
    Serial.println(F("[SD]:[SETUP] Root directory:"));
    root.ls(LS_R | LS_DATE | LS_SIZE);

    result = _file.open(root, filename, O_WRITE | O_CREAT);
    if (!result)
    {
        Serial.println(F("[SD]:[SETUP] Failed to open file for writing"));
        return false;
    }
    _file.println(SCHEMA_MSG);
    _file.flush();

    Serial.println(F("[SD]:[SETUP] Configured fine!"));
    return true;
}
