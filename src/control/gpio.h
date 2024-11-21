#ifndef GPIO_H
#define GPIO_H

#include "relaycontrol.h"

// Define relay GPIO pins
#define RLY1 17
#define RLY2 16
#define RLY3 4
#define RLY4 13
#define RLY5 14
#define RLY6 15

// Create Relay objects
Relay relay1(RLY1);
Relay relay2(RLY2);
Relay relay3(RLY3);
Relay relay4(RLY4);
Relay relay5(RLY5);
Relay relay6(RLY6);

// Function to test all relays sequentially
void testAllRelays() {
    relay1.on();
    delay(1000);
    relay1.off();

    relay2.on();
    delay(1000);
    relay2.off();

    relay3.on();
    delay(1000);
    relay3.off();

    relay4.on();
    delay(1000);
    relay4.off();

    relay5.on();
    delay(1000);
    relay5.off();

    relay6.on();
    delay(1000);
    relay6.off();
}
#endif