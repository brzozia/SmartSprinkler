#include "OutputModule.h"

OutputModule::OutputModule() 
{
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(SOIL_HUMIDITY_SENSOR, INPUT);
    digitalWrite(PUMP_PIN, LOW);
    pinMode(RGB_LED_R_PIN, OUTPUT);
    pinMode(RGB_LED_G_PIN, OUTPUT);
    pinMode(RGB_LED_B_PIN, OUTPUT);
    digitalWrite(RGB_LED_R_PIN, OUTPUT);
    digitalWrite(RGB_LED_G_PIN, OUTPUT);
    digitalWrite(RGB_LED_B_PIN, OUTPUT);
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

int OutputModule::read_humidity() 
{
    return analogRead(SOIL_HUMIDITY_SENSOR);
}
