#include "utils/scanutils.h"
#include "connect/connection.h"

#include "mcp/mcp23017_extend.h"
#include "sensors/sensor.h"

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");

  // scani2c();
  // setupMCP();
  // test1();

  // Setup WiFi
  WiFi.onEvent(WiFiEvent);
  initWifi();

  // Setup MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  // test2();
  // testpump();
  // testvalve2();
  // initData();
  if (!wifiConnected)
  {
    reconnectWifi(); // Try to reconnect to Wi-Fi
  }

  if (!client.connected())
  {
    reconnectMQTT();
  }
  client.loop();
}