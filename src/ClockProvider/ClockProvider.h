#ifndef __CLOCKPROVIER_H__
#define __CLOCKPROVIER_H__
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

class ClockProvider{
    private:
        const long utcOffsetInSeconds = 3600;
        unsigned long millisOffset;
        WiFiUDP ntpUDP;
        NTPClient timeClient;
    public:
        ClockProvider();
        void ntpSynchronize();
        unsigned long geTime();
        uint8_t getHours();
        uint8_t getMinutes();
        uint8_t getSeconds();
        void printTime();
        int elapsedInMinutes(unsigned long start_time);

};
#endif // __CLOCKPROVIER_H__