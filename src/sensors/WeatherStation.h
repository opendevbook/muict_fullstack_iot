#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <ModbusMaster.h>

class Weatherstation
{
private:
    uint8_t _id;
    String _name;
    ModbusMaster node;
    Stream *_Serial;
    uint8_t result;
    int16_t data;

public:
    Weatherstation(uint8_t id, String name, Stream *Serial)
    {
        _id = id;
        _name = name;
        _Serial = Serial;
    }

    void init()
    {
        Serial.println("Initializing Weather Station...");

        node.begin(_id, *_Serial);
        int attempts = 0;
        const int max_attempts = 10;

        while (attempts < max_attempts)
        {
            delay(1000);
            result = node.readHoldingRegisters(0x0160, 14);
            if (result == node.ku8MBSuccess)
            {
                Serial.println("[+] Successfully read data");
                return;
            }
            else
            {
                Serial.println("[-] Failed to read data, retrying...");
            }
            attempts++;
        }

        if (result != node.ku8MBSuccess)
        {
            Serial.println("[-] Unable to read data after retries.");
        }
    }

    // Individual getters for each register
    String getDeviceName()
    {
        return String(node.getResponseBuffer(0));
    }

    uint16_t getDataRate()
    {
        return node.getResponseBuffer(1);
    }

    uint16_t getDeviceAddress()
    {
        return node.getResponseBuffer(2);
    }

    uint16_t getDeviceInMSB()
    {
        return node.getResponseBuffer(3);
    }

    uint16_t getDeviceInLSB()
    {
        return node.getResponseBuffer(4);
    }

    uint16_t getLightData()
    {
        return node.getResponseBuffer(5);
    }

    uint16_t getUVIndex()
    {
        return node.getResponseBuffer(6);
    }

    float getTemperature()
    {
        return (node.getResponseBuffer(7) - 400) / 10.0; // Adjust offset and scale
    }

    float getHumidity()
    {
        return node.getResponseBuffer(8) / 10.0; // Convert to %RH
    }

    float getWindSpeed()
    {
        return node.getResponseBuffer(9) / 10.0; // Convert to m/s
    }

    float getGustSpeed()
    {
        return node.getResponseBuffer(10) / 10.0; // Convert to m/s
    }

    uint16_t getWindDirection()
    {
        return node.getResponseBuffer(11);
    }

    uint16_t getRainfall()
    {
        return node.getResponseBuffer(12);
    }

    float getAbsolutePressure()
    {
        return node.getResponseBuffer(13) / 10.0; // Convert to proper units
    }

    // Function to report all values
    void reportValues()
    {
        Serial.println("=== Weather Station Data ===");
        Serial.print("Device Name: ");
        Serial.println(getDeviceName());
        Serial.print("Data Rate: ");
        Serial.println(getDataRate());
        Serial.print("Device Address: ");
        Serial.println(getDeviceAddress());
        Serial.print("Device In MSB: ");
        Serial.println(getDeviceInMSB());
        Serial.print("Device In LSB: ");
        Serial.println(getDeviceInLSB());
        Serial.print("Light Data: ");
        Serial.println(getLightData());
        Serial.print("UV Index: ");
        Serial.println(getUVIndex());
        Serial.print("Temperature (°C): ");
        Serial.println(getTemperature());
        Serial.print("Humidity (%RH): ");
        Serial.println(getHumidity());
        Serial.print("Wind Speed (m/s): ");
        Serial.println(getWindSpeed());
        Serial.print("Gust Speed (m/s): ");
        Serial.println(getGustSpeed());
        Serial.print("Wind Direction: ");
        Serial.println(getWindDirection());
        Serial.print("Rainfall: ");
        Serial.println(getRainfall());
        Serial.print("Absolute Pressure (hPa): ");
        Serial.println(getAbsolutePressure());
        Serial.println("===========================");
    }
};

#endif
