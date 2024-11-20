#include <Adafruit_MCP23X17.h> // https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
                               // https://adafruit.github.io/Adafruit-MCP23017-Arduino-Library/html/class_adafruit___m_c_p23_x17.html

// Output

class MCPOutputConfigurator
{
private:
    Adafruit_MCP23X17 mcp;
    uint8_t address;

public:
    // Constructor to initialize with an I2C address
    explicit MCPOutputConfigurator(uint8_t i2cAddress) : address(i2cAddress) {}

    // Initialize the MCP23017
    bool begin()
    {
        Serial.printf("Initializing MCP23017 at address 0x%02X...\n", address);
        if (!mcp.begin_I2C(address))
        {
            Serial.println("Error: Failed to initialize MCP23017!");
            return false;
        }
        configureOutputMode();
        return true;
    }

    // Configure all pins as OUTPUT and set them to OFF
    void configureOutputMode()
    {
        Serial.printf("Configuring MCP23017 at address 0x%02X in OUTPUT mode\n", address);
        for (uint8_t i = 0; i < 16; i++)
        {
            mcp.pinMode(i, OUTPUT);
            mcp.digitalWrite(i, LOW); // Replace LOW with OFF if defined
        }
    }

    // Set the state of a specific pin
    void setPinState(uint8_t pin, bool state)
    {
        if (pin >= 16)
        {
            Serial.printf("Error: Pin %d out of range for MCP23017 at address 0x%02X\n", pin, address);
            return;
        }
        mcp.digitalWrite(pin, state ? HIGH : LOW);
        Serial.printf("Pin %d on MCP23017 0x%02X set to %s\n", pin, address, state ? "HIGH" : "LOW");
    }

    // Get the state of a specific pin
    bool getPinState(uint8_t pin)
    {
        if (pin >= 16)
        {
            Serial.printf("Error: Pin %d out of range for MCP23017 at address 0x%02X\n", pin, address);
            return false;
        }
        return mcp.digitalRead(pin);
    }

    // Turn all pins OFF (set to HIGH)
    void allPinOff()
    {
        Serial.printf("Turning all pins OFF for MCP23017 at address 0x%02X\n", address);
        for (uint8_t i = 0; i < 16; i++)
        {
            mcp.digitalWrite(i, HIGH);
        }
    }

    // Turn all pins ON (set to LOW)
    void allPinOn()
    {
        Serial.printf("Turning all pins ON for MCP23017 at address 0x%02X\n", address);
        for (uint8_t i = 0; i < 16; i++)
        {
            mcp.digitalWrite(i, LOW);
        }
    }

    // Get the MCP instance for advanced control
    Adafruit_MCP23X17 &getInstance()
    {
        return mcp;
    }
};

// input feedback
class MCPInputConfigurator
{
private:
    Adafruit_MCP23X17 mcp;
    uint8_t address;

public:
    MCPInputConfigurator(uint8_t addr) : address(addr) {}

    bool begin()
    {
        Serial.print("MCP23017 setup at address 0x");
        Serial.println(address, HEX);
        if (!mcp.begin_I2C(address))
        {
            Serial.print("Error initializing MCP23017 at address 0x");
            Serial.println(address, HEX);
            return false;
        }
        configureInputMode();
        return true;
    }

    void configureInterrupts(bool mirroring = true, bool openDrain = true, uint8_t polarity = LOW)
    {
        mcp.setupInterrupts(mirroring, openDrain, polarity);
    }

    void configurePin(uint8_t pin, uint8_t mode, uint8_t interruptType = CHANGE)
    {
        mcp.pinMode(pin, mode);
        if (mode == INPUT_PULLUP || mode == INPUT)
        {
            mcp.setupInterruptPin(pin, interruptType);
        }
    }

    // Configure all pins as OUTPUT and set them to OFF
    void configureInputMode()
    {
        // Configure global interrupt settings
        configureInterrupts(true, true, LOW);

        // Configure individual pins (example: PA0 to PA7 as INPUT_PULLUP with CHANGE interrupt)
        for (uint8_t pin = 0; pin < 8; pin++)
        {
            configurePin(pin, INPUT_PULLUP, CHANGE);     // PA0 to PA7
            configurePin(pin + 8, INPUT_PULLUP, CHANGE); // PB0 to PB7
        }
    }

    Adafruit_MCP23X17 &getMCP()
    {
        return mcp;
    }
};
