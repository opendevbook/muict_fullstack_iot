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

    // Store sensor data
    uint16_t lightData = 0;
    uint16_t uvIndex = 0;
    float temperature = 0.0;
    float humidity = 0.0;
    float windSpeed = 0.0;
    float gustSpeed = 0.0;
    uint16_t windDirection = 0;
    uint16_t rainfall = 0;
    float absolutePressure = 0.0;

public:
    Weatherstation(uint8_t id, String name, Stream *Serial)
        : _id(id), _name(name), _Serial(Serial) {}

    void init()
    {
        Serial.println("Initializing Weather Station...");
        node.begin(_id, *_Serial);

        const int maxAttempts = 10;
        int attempts = 0;

        while (attempts < maxAttempts)
        {
            delay(1000);
            result = node.readHoldingRegisters(0x0160, 14); // Read 14 registers
            if (result == node.ku8MBSuccess)
            {
                Serial.println("[+] Successfully initialized.");
                return;
            }
            else
            {
                Serial.println("[-] Failed to initialize, retrying...");
            }
            attempts++;
        }

        if (result != node.ku8MBSuccess)
        {
            Serial.println("[-] Unable to initialize after retries.");
        }
    }

    // Fetch and update all data from Modbus
    void fetchData()
    {
        if (result == node.ku8MBSuccess)
        {
            lightData = node.getResponseBuffer(5);
            uvIndex = node.getResponseBuffer(6);
            temperature = (node.getResponseBuffer(7) - 400) / 10.0;
            humidity = node.getResponseBuffer(8) / 10.0;
            windSpeed = node.getResponseBuffer(9) / 10.0;
            gustSpeed = node.getResponseBuffer(10) / 10.0;
            windDirection = node.getResponseBuffer(11);
            rainfall = node.getResponseBuffer(12);
            absolutePressure = node.getResponseBuffer(13) / 10.0;
        }
    }

    // Individual getters for sensor data
    uint16_t getLightData() { return lightData; }
    uint16_t getUVIndex() { return uvIndex; }
    float getTemperature() { return temperature; }
    float getHumidity() { return humidity; }
    float getWindSpeed() { return windSpeed; }
    float getGustSpeed() { return gustSpeed; }
    uint16_t getWindDirection() { return windDirection; }
    uint16_t getRainfall() { return rainfall; }
    float getAbsolutePressure() { return absolutePressure; }

    // Report all values
    void reportValues()
    {
        Serial.println("=== Weather Station Data ===");
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

    // Generate JSON payload for ThingsBoard
    String generatePayload()
    {
        fetchData(); // Ensure data is updated
        String payload = "{";
        payload += "\"lightData\":" + String(lightData) + ",";
        payload += "\"uvIndex\":" + String(uvIndex) + ",";
        payload += "\"temperature\":" + String(temperature, 2) + ",";
        payload += "\"humidity\":" + String(humidity, 2) + ",";
        payload += "\"windSpeed\":" + String(windSpeed, 2) + ",";
        payload += "\"gustSpeed\":" + String(gustSpeed, 2) + ",";
        payload += "\"windDirection\":" + String(windDirection) + ",";
        payload += "\"rainfall\":" + String(rainfall) + ",";
        payload += "\"absolutePressure\":" + String(absolutePressure, 2);
        payload += "}";
        return payload;
    }
};

#endif
