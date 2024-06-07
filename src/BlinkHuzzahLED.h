#include <Arduino.h>

#include "Behavior.h"

class BlinkHuzzahLED : virtual public Behavior {
    
    int blinkrate;

    public:
        BlinkHuzzahLED(int br = 500) {
            blinkrate = br;
            pinMode(0, OUTPUT);
        };

        void loop() override {
            if(isOn()) {
                digitalWrite(0, LOW);
                delay(blinkrate);
                digitalWrite(0, HIGH);
                delay(blinkrate);
            } else {
                digitalWrite(0, HIGH);
            }
        };
};