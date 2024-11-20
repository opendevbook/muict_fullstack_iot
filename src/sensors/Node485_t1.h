#ifndef Node485_t1_h
#define Node485_t1_h

#include <Arduino.h>
#include <ModbusMaster.h>

class Node485_t1
{
private:
  uint8_t _id;
  String _name;
  ModbusMaster node;
  Stream *_Serial;
  uint8_t result;
  int16_t data;
  int16_t Soil1 = 0;
  int16_t Soil2 = 0;
  int16_t Soil3 = 0;
  float Temp = 0;
  float Humd = 0;

public:
  Node485_t1(uint8_t id, String name, Stream *Serial)
  {
    _id = id;
    _name = name;
    _Serial = Serial;
  }

  void init()
  {

    Serial.println("┌------------------------------------------------┐");
    Serial.println("│ ** init Type     Komomi sensor Airtemp         │");
    Serial.println("└------------------------------------------------┘");
    node.begin(_id, *_Serial);
    int n = 0;
    int num_try = 10;
    do
    {
      delay(1000);
      result = node.readHoldingRegisters(0x0000, 5);
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
      Soil1 = node.getResponseBuffer(0x00);
      Soil2 = node.getResponseBuffer(0x01);
      Soil3 = node.getResponseBuffer(0x02);
      Temp = node.getResponseBuffer(0x03);
      Humd = node.getResponseBuffer(0x04);
    }
  }

  float getSoil1() { return Soil1; }

  float getSoil2() { return Soil2; }

  float getSoil3() { return Soil3; }

  float getTemp() { return Temp / 10; }

  float getHumid() { return Humd / 10; }

  void getString()
  {

    Serial.println(">>> Get string ");

    Serial.print("[+] Id: ");
    Serial.println(_id);
    Serial.print(", _name: ");
    Serial.println(_name);
    Serial.print("Soil1: ");
    Serial.println(Soil1);
    Serial.print("Soil2: ");
    Serial.println(Soil2);
    Serial.print("Soil3: ");
    Serial.println(Soil3);
    Serial.print("Temp: ");
    Serial.println(Temp);
    Serial.print("Humd: ");
    Serial.println(Humd);
  }
};

#endif
