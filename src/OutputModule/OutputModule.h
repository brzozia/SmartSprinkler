#ifndef __OUTPUTMODULE_H__
#define __OUTPUTMODULE_H__

#include <Arduino.h>


class OutputModule{
    public:
        void ledOff();
        void ledOn();
        void ledToggle();
        void pumpOn();
        void pumpOff();
        void ledSetColor(uint8_t r, uint8_t g, uint8_t b);
        int read_humidity();
        OutputModule();
    private:
        uint8_t ledR = 0;
        uint8_t ledG = 0;
        uint8_t ledB = 0;
        bool ledOnState;
        unsigned long pumpTimeOn = 0;
        bool dark_mode = false;

        //check if compiler optimizes that
        const int PUMP_PIN = 0;
        const int SOIL_HUMIDITY_SENSOR = 4;
        const int RGB_LED_R_PIN = 11;
        const int RGB_LED_G_PIN = 2;
        const int RGB_LED_B_PIN = 14;
};

#endif // __OUTPUTMODULE_H__