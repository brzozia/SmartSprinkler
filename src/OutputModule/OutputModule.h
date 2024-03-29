#ifndef __OUTPUTMODULE_H__
#define __OUTPUTMODULE_H__

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "../globals.h"
// #include "../GeneralLogger/GeneralLogger.h"

#define MAX_WATERING_TIME 60000

class OutputModule{
    public:
        void builtinLedOn();
        void builtinLedOff();
        void builtinLedToggle();
        void keepAliveBlink(bool on);
        void ledOff();
        void ledOn();
        void ledToggle();
        void pumpOn(int reason = -2);
        int getOnReason();
        void pumpOff();
        int getLeftTimeInSec();
        int pumpStatus();
        void pumpOnForTimeSec(unsigned long duration, int reason = -2);
        void ledSetColor(uint8_t r, uint8_t g, uint8_t b);
        void ledSetR(uint8_t v);
        void ledSetG(uint8_t v);
        void ledSetB(uint8_t v);
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
        DHT dht;
        bool ledOnState;
        unsigned long pumpTimeOn = 0;
        int pumpOnReason = -1;
        unsigned long wateringTime = 0;
        bool dark_mode = false;
        bool keepAliveBlinkState = true;

        //check if compiler optimizes that
        const uint8_t PUMP_PIN = 16;
        const uint8_t SOIL_HUMIDITY_SENSOR = A0;
        const uint8_t RGB_LED_R_PIN = 15;
        const uint8_t RGB_LED_G_PIN = 2;
        const uint8_t RGB_LED_B_PIN = 14;
        const uint8_t DHT_PIN = 5;
};

#endif // __OUTPUTMODULE_H__