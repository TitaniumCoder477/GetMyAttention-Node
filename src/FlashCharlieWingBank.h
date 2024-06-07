#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>

#include "Behavior.h"

class FlashCharlieWingBank : public Behavior {
    
    Adafruit_IS31FL3731 ledmatrix;
    int blinkrate;

    public:
        FlashCharlieWingBank(int brightness = 1, int br = 1000) {
            ledmatrix = Adafruit_IS31FL3731();
            blinkrate = br;
            if (! ledmatrix.begin()) {
                Serial.println("IS31 not found");
                while (1);
            }
            Serial.println("IS31 found!");
            
            ledmatrix.setFrame(0);
            for (uint8_t x=0; x<16; x++) {
                for (uint8_t y=0; y<9; y++) {
                    ledmatrix.drawPixel(x, y, 0);
                }
            }
            
            ledmatrix.setFrame(1);
            for (uint8_t x=0; x<16; x++) {
                for (uint8_t y=0; y<9; y++) {
                    ledmatrix.drawPixel(x, y, brightness);
                }
            }
        };

        void loop() override {
            if(isOn()) {
                ledmatrix.displayFrame(0);
                delay(blinkrate);
                ledmatrix.displayFrame(1);
                delay(blinkrate);
            } else {
                ledmatrix.displayFrame(0);
            }
        };
};