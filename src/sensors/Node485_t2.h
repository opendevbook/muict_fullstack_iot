#ifndef Node485_t2_h
#define Node485_t2_h

#include <Arduino.h>
#include <ModbusMaster.h>

class Node485_t2
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
  float Lux_float;

  uint32_t data_read32bit;
  uint16_t data_read16bit_H;
  uint16_t data_read16bit_L;

public:
  Node485_t2(uint8_t id, String name, Stream *Serial)
  {
    _id = id;
    _name = name;
    _Serial = Serial;
  }

  void init()
  {

    Serial.println("┌------------------------------------------------┐");
    Serial.println("│ ** init Type   kmmsensor: Light                │");
    Serial.println("└------------------------------------------------┘");
    node.begin(_id, *_Serial);
    int n = 0;
    int num_try = 10;
    do
    {
      delay(1000);
      result = node.readHoldingRegisters(0x0000, 7);
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
      Serial.println("[+] Success connect to node");
    }
    else if (result == 226)
    {
      Serial.println("[-] Fails connect to node");
    }
    if (result == 0)
    {
      // soil 1
      Soil1 = node.getResponseBuffer(0x00);
      Soil2 = node.getResponseBuffer(0x01);
      Soil3 = node.getResponseBuffer(0x02);

      data_read16bit_H = node.getResponseBuffer(0x05);
      data_read16bit_L = node.getResponseBuffer(0x06);
      data_read32bit = data_read16bit_H;
      data_read32bit = (data_read32bit << 16) | data_read16bit_L;
      int32_t data_read_int;

      data_read_int = *((uint32_t *)&data_read32bit);
      Lux_float = (float)data_read_int;
    }
  }

  float getSoil1() { return Soil1; }

  float getSoil2() { return Soil2; }

  float getSoil3() { return Soil3; }

  float getLux() { return Lux_float; }

  void getString()
  {
    Serial.println("┌------------------------------------------------┐");
    Serial.println("│ >> Get string                                  │");
    Serial.println("└------------------------------------------------┘");
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
    Serial.print("Lux_float: ");
    Serial.println(Lux_float);
  }
};

#endif
