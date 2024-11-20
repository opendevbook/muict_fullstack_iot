#ifndef SMHUMIDTEMPEC_H
#define SMHUMIDTEMPEC_H

#include <Arduino.h>
#include <ModbusMaster.h>

class SmHumidTempEc
{
private:
  uint8_t _id;
  String _name;
  ModbusMaster node;
  Stream *_Serial;
  uint8_t result;
  int16_t data;

  float ec_ec;
  float sm_h1;
  float sm_h2;
  float sm_t1;
  float sm_t2;

public:
  SmHumidTempEc(uint8_t id, String name, Stream *Serial)
  {
    _id = id;
    _name = name;
    _Serial = Serial;
  }

  void init()
  {
    Serial.println("┌------------------------------------------------┐");
    Serial.println("│ ** init Type        SmHumidTempEc              │");
    Serial.println("└------------------------------------------------┘");
    node.begin(_id, *_Serial);
    int n = 0;
    int num_try = 10;
    do
    {
      delay(1000);
      result = node.readHoldingRegisters(0x0000, 8);
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
      Serial.println("[+] Success connect to node : " + (String)_id);
    }
    else if (result == 226)
    {
      Serial.println("[-] Fails connect to node : " + (String)_id);
    }

    if (result == 0)
    {

      // SM Sensor probe
      sm_t2 = node.getResponseBuffer(0x00); // Word size (0.0 to 100.0)
      sm_h2 = node.getResponseBuffer(0x01); // Word size (-40.0 to 80.0)

      // EC Sensor probe
      sm_t1 = node.getResponseBuffer(0x02); // Word size (0.00 to 100.00)
      sm_h1 = node.getResponseBuffer(0x03); // Word size (-40.00 to 80.00)
      ec_ec = node.getResponseBuffer(0x04); // Word size (0 to 20000)
    }
  }

  float get_sm_t1()
  {
    return sm_t1;
  }

  float get_sm_h1()
  {
    return sm_h1;
  }

  float get_ec_ec()
  {
    return ec_ec;
  }

  float get_sm_t2()
  {
    return sm_t2;
  }

  float get_sm_h2()
  {
    return sm_h2;
  }

  void getString()
  {
    Serial.println("Get string SmHumidTempEc");
    Serial.print("[+] Id: ");
    Serial.println(_id);
    Serial.print(", _name: ");
    Serial.println(_name);
    // higher
    Serial.print("Temperature Higher SM = ");
    Serial.println(sm_t1);
    Serial.print("Humidity Higher SM = ");
    Serial.println(sm_h1);
    Serial.print("EC Higher SM = ");
    Serial.println(ec_ec);
    // lower
    Serial.print("Temperature Lower SM = ");
    Serial.println(sm_t2);
    Serial.print("Humidity Lower SM = ");
    Serial.println(sm_h2);
  }
};

#endif
