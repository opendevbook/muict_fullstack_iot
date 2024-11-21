#ifndef SENSER_H
#define SENSOR_H

#include "WeatherStation.h"
#include "TempHumid.h"

Weatherstation weather(144, "weather", &Serial); // wn90lp  model
TempHumid tempHumidSensor1(1, "soiltemp", &Serial);
TempHumid tempHumidSensor2(2, "soiltemp", &Serial);

/*
 * sensors/  sensor.h
 */
void initData()
{
    const char *topic = "v1/devices/me/telemetry";

    weather.init();
    // Report sensor data
    weather.reportValues();

    // Send data to ThingsBoard
    String payload_weather = weather.generatePayload();
    client.publish("v1/devices/me/telemetry", payload_weather.c_str());
    Serial.println("[+] Data sent to ThingsBoard: " + payload_weather);

    // tempHumiSensor1
    tempHumidSensor1.init();
    // Retrieve sensor data
    float temperature_s1 = tempHumidSensor1.getTemperature();
    float humidity_s1 = tempHumidSensor1.getHumidity();

    // Report data to Serial
    tempHumidSensor1.reportValues();

    // Publish to ThingsBoard
    String payload_s1 = "{";
    payload_s1 += "\"temperature_s1\":" + String(temperature_s1, 2) + ",";
    payload_s1 += "\"humidity_s1\":" + String(humidity_s1, 2);
    payload_s1 += "}";

    if (client.publish(topic, payload_s1.c_str()))
    {
        Serial.println("[+] Data Soil1 sent to ThingsBoard successfully.");
    }
    else
    {
        Serial.println("[-] Failed Soil1 to send data to ThingsBoard.");
    }

    // tempHumiSensor2
    tempHumidSensor2.init();
    // Retrieve sensor data
    float temperature_s2 = tempHumidSensor2.getTemperature();
    float humidity_s2 = tempHumidSensor2.getHumidity();

    // Report data to Serial
    tempHumidSensor2.reportValues();

    // Publish to ThingsBoard
    String payload_s2 = "{";
    payload_s2 += "\"temperature_s2\":" + String(temperature_s2, 2) + ",";
    payload_s2 += "\"humidity_s2\":" + String(humidity_s2, 2);
    payload_s2 += "}";

    if (client.publish(topic, payload_s2.c_str()))
    {
        Serial.println("[+] Data Soil2 sent to ThingsBoard successfully.");
    }
    else
    {
        Serial.println("[-] Failed Soil2 to send data to ThingsBoard.");
    }
}

void getDataSensor()
{
}
#endif