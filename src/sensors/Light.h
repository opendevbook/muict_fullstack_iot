#ifndef LIGHT_H
#define LIGHT_H


#include <Arduino.h>
#include <ModbusMaster.h>

class Light {
  private:
    uint8_t _id;
    String _name;
    ModbusMaster node;
    Stream*  _Serial;
    uint8_t result;
    int16_t data;

    uint32_t data_read32bit;
    uint16_t data_read16bit_H;
    uint16_t data_read16bit_L;
    float plight;
    String light;

  public:
    Light(uint8_t id, String name, Stream *Serial) {
      _id = id;
      _Serial = Serial;
      _name = name;
    }

    void init() {

      Serial.println("init Light");
      Serial.print("[+] Id: ");
      Serial.print(_id);
      Serial.print(", name: ");
      Serial.println(_name);
      node.begin(_id, *_Serial);
      for (int i = 0; i < 350; i++) delay(1);
      result = node.readHoldingRegisters(0x0000, 2);
      for (int i = 0; i < 350; i++) delay(1);
      Serial.print("[+] Result: ");
      Serial.println(result);

      if (result == 0) {
        data_read16bit_H = node.getResponseBuffer(0x00);
        data_read16bit_L = node.getResponseBuffer(0x01);
        data_read32bit  =  data_read16bit_H ;
        data_read32bit  = (data_read32bit << 16) | data_read16bit_L ;
        int32_t data_read_int;

        data_read_int = *((uint32_t *)&data_read32bit);
        plight = (float)data_read_int / 10;

      }
    }

    String getLight() {
      return String(plight);
    }

    void getString() {

      Serial.println("Get string Light ");
      Serial.print("[+] Id: ");
      Serial.println(_id);
      Serial.print("light: ");
      Serial.println(String(plight));;

    }
};





#endif
