#ifndef TEMPHUMID_H
#define TEMPHUMID_H

#include <Arduino.h>
#include <ModbusMaster.h>

class TempHumid
{
private:
  uint8_t _id;
  String _name;
  ModbusMaster node;
  Stream *_Serial;
  uint8_t result;

  float temperature = 0.0;
  float humidity = 0.0;

public:
  TempHumid(uint8_t id, String name, Stream *Serial)
  {
    _id = id;
    _name = name;
    _Serial = Serial;
  }

  void init()
  {
    Serial.println("┌------------------------------------------------┐");
    Serial.println("│ Initializing TempHumid Sensor                 │");
    Serial.println("└------------------------------------------------┘");

    Serial.print("[+] Id: ");
    Serial.println(_id);
    node.begin(_id, *_Serial);

    int attempts = 0;
    const int maxAttempts = 10;

    while (attempts < maxAttempts)
    {
      delay(1000);
      result = node.readHoldingRegisters(0x0000, 2); // Reading 2 registers (temperature and humidity)
      if (result == node.ku8MBSuccess)
      {
        Serial.println("[+] Successfully initialized sensor.");
        return;
      }
      else
      {
        Serial.println("[-] Failed to initialize sensor, retrying...");
      }
      attempts++;
    }

    Serial.println("[-] Unable to initialize sensor after retries.");
  }

  // Fetch temperature in °C
  float getTemperature()
  {
    if (result == node.ku8MBSuccess)
    {
      temperature = node.getResponseBuffer(0x01) / 10.0; // Divide by 10 for correct scale
    }
    return temperature;
  }

  // Fetch humidity in %RH
  float getHumidity()
  {
    if (result == node.ku8MBSuccess)
    {
      humidity = node.getResponseBuffer(0x00) / 10.0; // Divide by 10 for correct scale
    }
    return humidity;
  }

  void reportValues()
  {
    Serial.println("┌------------------------------------------------┐");
    Serial.println("│ TempHumid Sensor Data                          │");
    Serial.println("└------------------------------------------------┘");

    Serial.print("Temperature (°C): ");
    Serial.println(getTemperature(), 2);
    Serial.print("Humidity (%RH): ");
    Serial.println(getHumidity(), 2);
  }
};

#endif
