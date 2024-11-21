#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include <Arduino.h>

class Relay
{
private:
    int pin;    // GPIO pin for the relay
    bool state; // Current state of the relay (true = ON, false = OFF)

public:
    // Constructor
    Relay(int relayPin)
    {
        pin = relayPin;
        state = false; // Default state is OFF
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH); // Ensure relay is off (active-low logic)
    }

    // Turn the relay ON
    void on()
    {
        digitalWrite(pin, LOW); // Active-low logic
        state = true;
    }

    // Turn the relay OFF
    void off()
    {
        digitalWrite(pin, HIGH); // Active-low logic
        state = false;
    }

    // Toggle the relay state
    void toggle()
    {
        state ? off() : on();
    }

    // Get the current state
    bool getState() const
    {
        return state;
    }
};

#endif
