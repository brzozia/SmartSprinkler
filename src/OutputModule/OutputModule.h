#ifndef __OUTPUTMODULE_H__
#define __OUTPUTMODULE_H__

#include <Arduino.h>

#define MAX_WATERING_TIME 10000

class OutputModule{
    public:
        void builtinLedOn();
        void builtinLedOff();
        void builtinLedToggle();
        void keepAliveBlink(bool on);
        void ledOff();
        void ledOn();
        void ledToggle();
        void pumpOn();
        void pumpOff();
        void pumpOnForTime(unsigned long duration);
        void ledSetColor(uint8_t r, uint8_t g, uint8_t b);
        float readAirHumidity();
        float readSoilHumidity();
        float readAirTemp();
        void tick();
        void pumpOffCheck();
        OutputModule();
    private:
        uint8_t ledR = 0;
        uint8_t ledG = 0;
        uint8_t ledB = 0;
        bool ledOnState;
        unsigned long pumpTimeOn = 0;
        unsigned long wateringTime = 0;
        bool dark_mode = false;
        bool keepAliveBlinkState = true;

        //check if compiler optimizes that
        const uint8_t PUMP_PIN = 0;
        const uint8_t SOIL_HUMIDITY_SENSOR = 4;
        const uint8_t RGB_LED_R_PIN = 11;
        const uint8_t RGB_LED_G_PIN = 2;
        const uint8_t RGB_LED_B_PIN = 14;
};

#endif // __OUTPUTMODULE_H__