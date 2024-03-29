
#include "ClockProvider.h"
ClockProvider::ClockProvider():timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds)
{
    timeClient.begin();
}

void ClockProvider::ntpSynchronize() 
{
    logger->notice("time synchronized with ntp\r\n");
    timeClient.update();
    setTime(timeClient.getEpochTime());
}

unsigned long ClockProvider::geTime() 
{
    return now();
}

uint8_t ClockProvider::getHours() 
{
    time_t t = now();
    return hour(t);
}

uint8_t ClockProvider::getMinutes() 
{
    time_t t = now();
    return minute(t);    
}

uint8_t ClockProvider::getSeconds() 
{
    time_t t = now();
    return second(t);    
}

void ClockProvider::printTime() 
{
    Serial.printf("%d:%d:%d \r\n", getHours(), getMinutes(), getSeconds());
}

