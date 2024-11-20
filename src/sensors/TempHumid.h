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
  int16_t data;
  int16_t pTemp = 0;
  int16_t pHumi = 0;
  int16_t Soil3 = 0;
  String Temp;
  String Humid;
  float TempFloat;
  float HumidFloat;

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
    Serial.println("│ init TempHumid                                 │");
    Serial.println("└------------------------------------------------┘");

    Serial.print("[+] Id: ");
    Serial.println(_id);
    node.begin(_id, *_Serial);
    int n = 0;
    int num_try = 10;
    do
    {
      delay(1000);
      result = node.readHoldingRegisters(0x0000, 2);
      if (result == node.ku8MBSuccess)

      {
        Serial.println("[+] Success to read");
        break;
      }
      else
      {
        Serial.println("[+] Fail to read");
      }
      n++;
    } while (n < num_try);

    char buffer[100];
    if (n > 1)
    {
      sprintf(buffer, "Try  Type1 %d time", n);
      Serial.println(buffer);
    }

    if (result == 0)
    {
      HumidFloat = node.getResponseBuffer(0x00);
      TempFloat = node.getResponseBuffer(0x01);
    }
  }

  float getTemp()
  {
    return (TempFloat * 1) / 10.00;
  }

  float getHumid()
  {
    return (HumidFloat * 1) / 10.00;
  }

  void getString()
  {
    Serial.println("┌------------------------------------------------┐");
    Serial.println("│ Get string Temp Humid                          │");
    Serial.println("└------------------------------------------------┘");
    Serial.print("[+] Id: ");
    Serial.println(_id);
    Serial.print(", _name: ");
    Serial.println(_name);
    Serial.print("Temp: ");
    Serial.println(TempFloat);
    Serial.print("Humid: ");
    Serial.println(HumidFloat);
  }
};

#endif
