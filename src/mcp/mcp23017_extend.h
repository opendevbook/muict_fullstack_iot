#ifndef MCP_H
#define MCP_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "mcp23017wrapper.h"

#define ON true
#define OFF false

#define LEFT true
#define RIGHT false

#define CLOSE true
#define OPEN false

// Define pin
byte mcp_pin_PA0 = 0;
byte mcp_pin_PA1 = 1;
byte mcp_pin_PA2 = 2;
byte mcp_pin_PA3 = 3;
byte mcp_pin_PA4 = 4;
byte mcp_pin_PA5 = 5;
byte mcp_pin_PA6 = 6;
byte mcp_pin_PA7 = 7;

byte mcp_pin_PB0 = 8;
byte mcp_pin_PB1 = 9;
byte mcp_pin_PB2 = 10;
byte mcp_pin_PB3 = 11;
byte mcp_pin_PB4 = 12;
byte mcp_pin_PB5 = 13;
byte mcp_pin_PB6 = 14;
byte mcp_pin_PB7 = 15;

byte VALVE1 = 0;
byte VALVE2 = 1;
byte VALVE3 = 2;
byte VALVE4 = 3;
byte VALVE5 = 4;
byte VALVE6 = 5;
byte VALVE7 = 6;
byte VALVE8 = 7;
byte VALVE9 = 8;
byte VALVE10 = 9;
byte VALVE11 = 9;

MCPInputConfigurator mcp1(0x21);
MCPInputConfigurator mcp2(0x22);
MCPInputConfigurator mcp3(0x23);
MCPInputConfigurator mcp4(0x24);

MCPOutputConfigurator mcp0(0x20);
MCPOutputConfigurator mcp5(0x25);
MCPOutputConfigurator mcp6(0x26);

// prototype

// implement
void setupMCP()
{
    Wire.begin();

    if (!mcp5.begin() || !mcp6.begin())
    {
        Serial.println("Error: One or more MCPOutputConfigurators failed to initialize!");
    }

    if (!mcp1.begin() || !mcp2.begin() || !mcp3.begin() || !mcp4.begin())
    {
        Serial.println("Error: One or more MCPInputConfigurators failed to initialize!");
    }
}

void test1()
{
    mcp5.allPinOff(); // 0x25 switch
    mcp6.allPinOn();  // 0x26
}

void pumpControl(uint8_t pin, bool state)
{
    // Log the action
    Serial.printf("Turning %s pump on pin %d...\n", state ? "ON" : "OFF", pin);

    // Set the pin state
    mcp6.setPinState(pin, state ? LOW : HIGH); // Assuming LOW is ON and HIGH is OFF
    delay(1000);

    // Log completion
    Serial.printf("Pump on pin %d is now %s.\n", pin, state ? "ON" : "OFF");
}

void pump1Control(bool state)
{
    pumpControl(mcp_pin_PB2, state);
}

void pump2Control(bool state)
{
    pumpControl(mcp_pin_PB3, state);
}

void test2()
{

    // control Right LCD is Green
    mcp5.setPinState(0, 1);
    mcp6.setPinState(0, 1);
    delay(1000);

    mcp5.setPinState(0, 0);
    mcp6.setPinState(0, 1);
    delay(1000);

    // control Left, LCD is Red
    mcp5.setPinState(0, 1);
    mcp6.setPinState(0, 1);
    delay(1000);

    mcp5.setPinState(0, 0);
    mcp6.setPinState(0, 0);
    delay(1000);
}

void testpump()
{
    pump1Control(ON);
    pump1Control(OFF);

    pump2Control(ON);
    pump2Control(OFF);
}

void controlvalve1(bool direction)
{
    if (direction == RIGHT)
    {
        // Control Right, LCD is Green
        mcp5.setPinState(0, HIGH); // Activate mcp5
        mcp6.setPinState(0, HIGH); // Activate mcp6
        delay(1000);

        mcp5.setPinState(0, LOW);  // Deactivate mcp5
        mcp6.setPinState(0, HIGH); // Keep mcp6 active
        delay(1000);
    }
    else if (direction == LEFT)
    {
        // Control Left, LCD is Red
        mcp5.setPinState(0, HIGH); // Activate mcp5
        mcp6.setPinState(0, HIGH); // Activate mcp6
        delay(1000);

        mcp5.setPinState(0, LOW); // Deactivate mcp5
        mcp6.setPinState(0, LOW); // Deactivate mcp6
        delay(1000);
    }
}

void controlvalve(uint8_t pin, bool direction)
{
    if (direction == RIGHT)
    {
        // Control Right, LCD is Green
        mcp5.setPinState(pin, LOW);  // Activate mcp5
        mcp6.setPinState(pin, HIGH); // Activate mcp6
        delay(15000);

        mcp5.setPinState(pin, HIGH); // Deactivate mcp5
        mcp6.setPinState(pin, HIGH); // Keep mcp6 active
        delay(1000);
    }
    else if (direction == LEFT)
    {
        // Control Left, LCD is Red
        mcp5.setPinState(pin, LOW); // Activate mcp5
        mcp6.setPinState(pin, LOW); // Activate mcp6
        delay(15000);

        mcp5.setPinState(pin, HIGH); // Deactivate mcp5
        mcp6.setPinState(pin, HIGH); // Deactivate mcp6
        delay(1000);
    }
}

void testvalve2()
{
    controlvalve(VALVE1, OPEN);
    delay(1000);
    controlvalve(VALVE1, CLOSE);
    delay(1000);

    controlvalve(VALVE2, OPEN);
    delay(1000);
    controlvalve(VALVE2, CLOSE);
    delay(1000);

    controlvalve(VALVE3, OPEN);
    delay(1000);
    controlvalve(VALVE3, CLOSE);
    delay(1000);

    controlvalve(VALVE4, OPEN);
    delay(1000);
    controlvalve(VALVE4, CLOSE);
    delay(1000);

    controlvalve(VALVE5, OPEN);
    delay(1000);
    controlvalve(VALVE5, CLOSE);
    delay(1000);

    controlvalve(VALVE6, OPEN);
    delay(1000);
    controlvalve(VALVE6, CLOSE);
    delay(1000);

    controlvalve(VALVE7, OPEN);
    delay(1000);
    controlvalve(VALVE7, CLOSE);
    delay(1000);

    controlvalve(VALVE8, OPEN);
    delay(1000);
    controlvalve(VALVE8, CLOSE);
    delay(1000);

    controlvalve(VALVE9, OPEN);
    delay(1000);
    controlvalve(VALVE9, CLOSE);
    delay(1000);

    controlvalve(VALVE10, OPEN);
    delay(1000);
    controlvalve(VALVE10, CLOSE);
    delay(1000);

    controlvalve(VALVE11, OPEN);
    delay(1000);
    controlvalve(VALVE11, CLOSE);
    delay(1000);
}

#endif
