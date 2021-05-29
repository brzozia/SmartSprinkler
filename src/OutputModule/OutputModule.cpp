#include "OutputModule.h"

OutputModule::OutputModule() 
{
    pinMode(BUILTIN_LED , OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
    // pinMode(PUMP_PIN, OUTPUT);
    // pinMode(SOIL_HUMIDITY_SENSOR, INPUT);
    // digitalWrite(PUMP_PIN, LOW);
    // pinMode(RGB_LED_R_PIN, OUTPUT);
    // pinMode(RGB_LED_G_PIN, OUTPUT);
    // pinMode(RGB_LED_B_PIN, OUTPUT);
    // digitalWrite(RGB_LED_R_PIN, OUTPUT);
    // digitalWrite(RGB_LED_G_PIN, OUTPUT);
    // digitalWrite(RGB_LED_B_PIN, OUTPUT);
}

void OutputModule::ledOff() 
{
    ledOnState = false;
    analogWrite(RGB_LED_R_PIN, 0);
    analogWrite(RGB_LED_G_PIN, 0);
    analogWrite(RGB_LED_B_PIN, 0);
}

void OutputModule::ledOn() 
{
    ledOnState = true;
    analogWrite(RGB_LED_R_PIN, ledR);
    analogWrite(RGB_LED_G_PIN, ledG);
    analogWrite(RGB_LED_B_PIN, ledB);    
}

void OutputModule::ledToggle() 
{
    if(ledOnState) ledOff();
    else ledOn();
}

void OutputModule::pumpOn() 
{
    pumpTimeOn = millis();
    digitalWrite(PUMP_PIN, HIGH);
}

void OutputModule::pumpOff() 
{
    digitalWrite(PUMP_PIN, LOW);
}

void OutputModule::pumpOnForTime(unsigned long duration) 
{
    wateringTime = duration;
    pumpOn();
}

void OutputModule::ledSetColor(uint8_t r, uint8_t g, uint8_t b) 
{

    ledR = r;
    ledG = g;
    ledB = b;
    if(ledOnState){
        analogWrite(RGB_LED_R_PIN, ledR);
        analogWrite(RGB_LED_G_PIN, ledG);
        analogWrite(RGB_LED_B_PIN, ledB);        
    }

    
}

float OutputModule::readAirHumidity() 
{
    return 78.5;
}

float OutputModule::readSoilHumidity() 
{
    return 34.4;
}

float OutputModule::readAirTemp() 
{
    return 22.2;
}

void OutputModule::tick() 
{
    if(keepAliveBlinkState){
        builtinLedToggle();
    }
}

void OutputModule::pumpOffCheck() 
{
    if(pumpTimeOn + wateringTime < millis()){
        pumpOff();
    }
    if(pumpTimeOn + MAX_WATERING_TIME < millis()){
        pumpOff();
    }
}

void OutputModule::builtinLedOn() 
{
    digitalWrite(BUILTIN_LED, HIGH);
}

void OutputModule::builtinLedOff() 
{
    digitalWrite(BUILTIN_LED, LOW);
}

void OutputModule::builtinLedToggle() 
{
    if(digitalRead(BUILTIN_LED)) builtinLedOff();
    else builtinLedOn();
}

void OutputModule::keepAliveBlink(bool on) 
{
    keepAliveBlinkState = on;
}
