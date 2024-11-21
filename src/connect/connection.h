#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi Credentials
// const char *ssid = "itbakery-wifi";
const char *ssid = "TrueGigatexFiber_uS7_2.4G";
const char *password = "itbakery@9";

// const char *access_token = "QBxa2YOnjquSc1qb5R1v";
// ThingsBoard server and token
const char *mqtt_server = "demo.thingsboard.io";
const char *access_token = "4KZLx8HMR4E4nVfriEQL";

const int mqtt_port = 1883;

// MQTT Client

WiFiClient espClient;
PubSubClient client(espClient);

// Function open prototypes
void OpenValve1();
void OpenValve2();
void OpenValve3();
void OpenValve4();
void OpenValve5();
void OpenValve6();
void OpenValve7();
void OpenValve8();
void OpenValve9();
void OpenValve10();
void OpenValve11();

// Function open prototypes
void CloseValve1();
void CloseValve2();
void CloseValve3();
void CloseValve4();
void CloseValve5();
void CloseValve6();
void CloseValve7();
void CloseValve8();
void CloseValve9();
void CloseValve10();
void CloseValve11();

// Array of device state keys
const char *deviceKeys[11] = {
    "deviceState1", "deviceState2", "deviceState3",
    "deviceState4", "deviceState5", "deviceState6",
    "deviceState7", "deviceState8", "deviceState9", "deviceState10", "deviceState11"};

// Array of function pointers corresponding to each device state
void (*OpenDeviceFunctions[11])() = {
    OpenValve1, OpenValve2, OpenValve3,
    OpenValve4, OpenValve5, OpenValve6,
    OpenValve7, OpenValve8, OpenValve9,
    OpenValve10, OpenValve11};

void (*CloseDeviceFunctions[11])() = {
    CloseValve1, CloseValve2, CloseValve3,
    CloseValve4, CloseValve5, CloseValve6,
    CloseValve7, CloseValve8, CloseValve9,
    CloseValve10, CloseValve11};

// Flag to track Wi-Fi connection status
bool wifiConnected = false;

// Prototype
void initWifi();
void requestSharedAttributes();
void handleSharedAttributes(const String &message);
void handleRpcRequest(const String &topic, const String &message);
void handleAttribute(const String &message);
void handleDeviceState(const String &key, const String &state);
void callback(char *topic, byte *payload, unsigned int length);

// Function to initialize WiFi connection and display Wi-Fi information
void initWifi()
{
    Serial.println("\nConnecting to WiFi...");
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("WiFi connected!");

    // Print Wi-Fi information
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI()); // Signal strength in dBm
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // ESP32 IP address
    Serial.print("Channel: ");
    Serial.println(WiFi.channel()); // Wi-Fi channel number
}

// Wi-Fi event handler function
void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_CONNECTED:
        Serial.println("\nWiFi Connected");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("\nWiFi Disconnected, attempting to reconnect...");
        wifiConnected = false; // Set flag to false when disconnected
        break;
    default:
        break;
    }
}

void requestSharedAttributes()
{
    // Log the start of the request
    Serial.println("\nProcessing Request for Shared Attributes...");
    const char *requestTopic = "v1/devices/me/attributes/request/1"; // Replace '1' with your request ID
    const char *payload = "{}";                                      // Empty JSON to request all shared attributes
    Serial.println("Requesting all shared attributes...");
    bool success = client.publish(requestTopic, payload);
    if (success)
    {
        Serial.println("+Request sent successfully.");
    }
    else
    {
        Serial.println("-Failed to send request. Check MQTT connection.");
    }
}

// Funtion connect to mqtt
void connectToMQTT()
{
    const int maxRetries = 10; // Maximum number of retries
    int tryNum = 0;            // Current attempt count

    while (!client.connected() && tryNum < maxRetries)
    {
        Serial.printf("Connecting to MQTT... Attempt %d/%d\n", tryNum + 1, maxRetries);

        // Try to connect to the MQTT broker
        if (client.connect("ESP32", access_token, nullptr))
        {
            Serial.println("Connected to MQTT broker.");
            // Once connected, subscribe or publish to topics
            // Subscribe to required topics and check results
            if (client.subscribe("v1/devices/me/rpc/request/+"))
            {
                Serial.println("+Subscribed to RPC request topic successfully.");
            }
            else
            {
                Serial.println("-Failed to subscribe to RPC request topic.");
            }

            if (client.subscribe("v1/devices/me/attributes"))
            {
                Serial.println("+Subscribed to attributes topic successfully.");
            }
            else
            {
                Serial.println("-Failed to subscribe to attributes topic.");
            }

            if (client.subscribe("v1/devices/me/attributes/response/+"))
            {
                Serial.println("+Subscribed to attributes response topic successfully.");
            }
            else
            {
                Serial.println("-Failed to subscribe to attributes response topic.");
            }

            // Request shared attributes after subscribing

            requestSharedAttributes();
            return; // Exit the function as we are connected
        }
        else
        {
            Serial.printf("Failed to connect, rc=%d. Retrying in 5 seconds...\n", client.state());
            delay(5000); // Wait for 5 seconds before trying again
            tryNum++;    // Increment attempt count
        }
    }

    // If we exit the loop without connecting, print a failure message
    if (!client.connected())
    {
        Serial.println("Failed to connect to MQTT broker after maximum retries. Exiting.");
    }
}

// Function to reconnect to Wi-Fi with retry limit
void reconnectWifi()
{
    int try_num = 10; // Maximum number of retry attempts
    int attempts = 0; // Attempt counter

    Serial.println("\nAttempting to reconnect to WiFi...");

    // Try to reconnect up to 'try_num' times
    while (WiFi.status() != WL_CONNECTED && attempts < try_num)
    {
        Serial.print(".");
        WiFi.begin(ssid, password); // Try to reconnect
        delay(5000);                // Wait for 5 seconds before trying again
        attempts++;                 // Increment the attempt counter
        Serial.print(attempts);
    }

    // Check if reconnected
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Reconnected to WiFi!");
        wifiConnected = true; // Set flag to true once reconnected
    }
    else
    {
        Serial.println("Failed to reconnect to WiFi after 10 attempts.");
        wifiConnected = false; // Set flag to false if reconnection fails
    }
}

// Callback for incoming MQTT messages
void callback(char *topic, byte *payload, unsigned int length)
{

    // Convert the char* topic to String
    String topicStr(topic);

    // Get the current time for logging (optional)
    unsigned long timestamp = millis();

    // Print the log with a timestamp, topic, and payload message
    Serial.print("\n[INFORMATION  CALL BACK]\n ");
    Serial.print("Time: ");
    Serial.print(timestamp); // Prints the time since the program started in milliseconds
    Serial.print(" ms \n| ");

    // Format topic display
    Serial.print("Topic: ");
    Serial.print(topicStr);
    Serial.print(" \n| ");

    // Print the message length
    Serial.print("Message Length: ");
    Serial.print(length);
    Serial.print(" bytes | ");

    String message;
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }
    Serial.println(message);

    // 1 Handle RPC requests
    if (String(topic).startsWith("v1/devices/me/rpc/request/"))
    {
        Serial.println("\n1 RPC request received:");
        handleRpcRequest(topic, message);
    }

    // 2 Handle Shared Attributes
    if (String(topic).equals("v1/devices/me/attributes"))
    {
        Serial.println("\n2 Shared attributes updated");
        Serial.println("Attributes: " + message);
        // Process shared attributes
        handleAttribute(message);
    }

    // 3 Handle the response to the shared attribute request
    if (String(topic).equals("v1/devices/me/attributes/response/1"))
    {
        Serial.println("\n3 Shared attributes response received:");
        Serial.println(message);
        // Process the shared attributes (JSON parsing, etc.)
        handleSharedAttributes(message);
    }
}

// MQTT Connection Setup
void reconnectMQTT()
{
    int retryCount = 0;
    while (!client.connected() && retryCount < 10)
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32", access_token, nullptr))
        {
            Serial.println("connected  to MQTT broker.");
            client.subscribe("v1/devices/me/rpc/request/+");
            client.subscribe("v1/devices/me/attributes");
            client.subscribe("v1/devices/me/attributes/response/+");
            requestSharedAttributes();
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
            retryCount++;
        }
    }
    if (retryCount == 10)
    {
        Serial.println("Failed to connect to MQTT broker. Restarting...");
        ESP.restart();
    }
}

/*
 *
 */
void handleSharedAttributes(const String &message)
{
    Serial.println("\nProcessing shared attributes...");

    // Allocate a JSON document based on expected payload size
    JsonDocument doc;

    // Deserialize JSON message
    DeserializationError error = deserializeJson(doc, message);

    // Handle deserialization errors
    if (error)
    {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Check if the shared attributes exist
    if (doc.containsKey("shared"))
    {
        JsonObject shared = doc["shared"];

        // Iterate through all shared attribute key-value pairs
        for (JsonPair kv : shared)
        {
            const char *key = kv.key().c_str();
            JsonVariant value = kv.value();
            Serial.println("\nDebug Shared attributed");
            Serial.print("Received Attribute: ");
            Serial.print(key);
            Serial.print(" = ");
            Serial.println(value.as<String>()); // Print the value (as string for simplicity)

            // Perform actions based on specific keys
            handleDeviceState(key, value);
        }
    }
    else
    {
        Serial.println("No 'shared' key found in the received attributes.");
    }
}

void handleRpcRequest(const String &topic, const String &message)
{
    Serial.println("Processing RPC request...");

    // Extract the RPC request ID from the topic
    const String rpcPrefix = "v1/devices/me/rpc/request/";
    if (!topic.startsWith(rpcPrefix))
    {
        Serial.println("Invalid RPC topic format.");
        return;
    }

    String requestId = topic.substring(rpcPrefix.length());
    Serial.print("RPC Request ID: ");
    Serial.println(requestId);

    // Parse the JSON message
    JsonDocument doc; // Adjust size as necessary
    DeserializationError error = deserializeJson(doc, message);

    // Handle deserialization errors
    if (error)
    {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Check if the JSON contains a "method" key
    if (!doc.containsKey("method"))
    {
        Serial.println("No 'method' key found in the RPC payload.");
        return;
    }

    String method = doc["method"].as<String>();
    Serial.print("RPC Method: ");
    Serial.println(method);

    // Perform actions based on the method
    if (method == "turnOn")
    {
        Serial.println("Executing 'turnOn' command...");
        // Perform actions to turn on the device
        client.publish(("v1/devices/me/rpc/response/" + requestId).c_str(), "{\"status\":\"success\"}");
    }
    else if (method == "turnOff")
    {
        Serial.println("Executing 'turnOff' command...");
        // Perform actions to turn off the device
        client.publish(("v1/devices/me/rpc/response/" + requestId).c_str(), "{\"status\":\"success\"}");
    }
    else
    {
        Serial.println("Unknown RPC method.");
        // Respond with an error
        client.publish(("v1/devices/me/rpc/response/" + requestId).c_str(), "{\"error\":\"Unknown method\"}");
    }

    // Optional: Handle additional parameters if they exist
    if (doc.containsKey("params"))
    {
        Serial.println("RPC Parameters:");
        serializeJsonPretty(doc["params"], Serial);
        Serial.println();
        // Process parameters as needed
    }
}

void handleAttribute(const String &message)
{
    // Log that an attribute message has been received
    Serial.println("\nProcessing Single Attribute...");

    // Create a StaticJsonDocument to parse the incoming JSON message
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);

    // Check if there was an error during JSON deserialization
    if (error)
    {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.c_str());
        return; // Exit if JSON parsing fails
    }

    // Now the document should contain the key-value pairs. Iterate over the keys.
    for (JsonPair kv : doc.as<JsonObject>())
    {
        // Extract the key
        String key = kv.key().c_str();

        // Extract the value
        JsonVariant value = kv.value();

        // Log the key and value received
        Serial.print("Received Attribute: ");
        Serial.print(key);
        Serial.print(" = ");
        Serial.println(value.as<String>()); // Print the value (as string for simplicity)

        // Handle the attribute based on the key and value
        handleDeviceState(key, value);
    }
}

// Function to handle device state based on the key
void handleDeviceState(const String &key, const String &state)
{
    Serial.println("\nProcess handle DeviceState");
    for (int i = 0; i < 11; i++)
    {
        if (key == deviceKeys[i])
        {
            // Perform action based on device state
            Serial.print("Updating attribute ");
            Serial.print(deviceKeys[i]);
            Serial.print(" to ");
            bool stateBool = (state == "true");
            Serial.println(stateBool ? "ON" : "OFF");

            // map to function
            if (stateBool)
            {
                Serial.println("\nCall Open function");
                OpenDeviceFunctions[i](); // Call Open function
            }
            else
            {
                Serial.println("\nCall Close function");
                CloseDeviceFunctions[i](); // Call closs function
            }
            break;
        }
        else
        {
            Serial.println("Unknown device key!");
        }
    }
}

// Example functions to open the valves
void OpenValve1()
{
    Serial.println("Opening Valve 1");
    // controlvalve(VALVE1, OPEN);
    relay1.on();
}
void OpenValve2()
{
    Serial.println("Opening Valve 2");
    // controlvalve(VALVE2, OPEN);
    relay2.on();
}
void OpenValve3()
{
    Serial.println("Opening Valve 3");
    // controlvalve(VALVE3, OPEN);
    relay3.on();
}
void OpenValve4()
{
    Serial.println("Opening Valve 4");
    // controlvalve(VALVE4, OPEN);
    relay4.on();
}
void OpenValve5()
{
    Serial.println("Opening Valve 5");
    // controlvalve(VALVE5, OPEN);
    relay5.on();
}
void OpenValve6()
{
    Serial.println("Opening Valve 6");
    // controlvalve(VALVE6, OPEN);
    relay6.on();
}
void OpenValve7()
{
    Serial.println("Opening Valve 7");
    // controlvalve(VALVE7, OPEN);
}
void OpenValve8()
{
    Serial.println("Opening Valve 8");
    // controlvalve(VALVE8, OPEN);
}
void OpenValve9()
{
    Serial.println("Opening Valve 9");
    // controlvalve(VALVE9, OPEN);
}
void OpenValve10()
{
    Serial.println("Opening - Pump1");
    // controlvalve(VALVE10, OPEN);
    // pump1Control(ON);
}
void OpenValve11()
{
    Serial.println("Opening  Pump2");
    // controlvalve(VALVE11, OPEN);
    // pump2Control(ON);
}

// Example functions to Close the valves
void CloseValve1()
{
    Serial.println("Closeing Valve 1");
    // controlvalve(VALVE1, CLOSE);
    relay1.off();
}
void CloseValve2()
{
    Serial.println("Closeing Valve 2");
    // controlvalve(VALVE2, CLOSE);
    relay2.off();
}
void CloseValve3()
{
    Serial.println("Closeing Valve 3");
    // controlvalve(VALVE3, CLOSE);
    relay3.off();
}
void CloseValve4()
{
    Serial.println("Closeing Valve 4");
    // controlvalve(VALVE4, CLOSE);
    relay4.off();
}
void CloseValve5()
{
    Serial.println("Closeing Valve 5");
    // controlvalve(VALVE5, CLOSE);
    relay5.off();
}
void CloseValve6()
{
    Serial.println("Closeing Valve 6");
    // controlvalve(VALVE6, CLOSE);
    relay6.off();
}
void CloseValve7()
{
    Serial.println("Closeing Valve 7");
    // controlvalve(VALVE7, CLOSE);
}
void CloseValve8()
{
    Serial.println("Closeing Valve 8");
    // controlvalve(VALVE8, CLOSE);
}
void CloseValve9()
{
    Serial.println("Closeing Valve 9");
    // controlvalve(VALVE9, CLOSE);
}
void CloseValve10()
{
    Serial.println("Closeing  Pump1");
    // controlvalve(VALVE10, OPEN);
    // pump1Control(OFF);
}
void CloseValve11()
{
    Serial.println("Closeing Pump2");
    // controlvalve(VALVE11, OPEN);
    // pump2Control(OFF);
}

#endif