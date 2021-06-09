#include "OutputModule.h"

OutputModule::OutputModule():dht(5, DHT11)
{
    pinMode(BUILTIN_LED , OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(SOIL_HUMIDITY_SENSOR, INPUT);
    digitalWrite(PUMP_PIN, LOW);
    pinMode(RGB_LED_R_PIN, OUTPUT);
    pinMode(RGB_LED_G_PIN, OUTPUT);
    pinMode(RGB_LED_B_PIN, OUTPUT);
    digitalWrite(RGB_LED_R_PIN, OUTPUT);
    digitalWrite(RGB_LED_G_PIN, OUTPUT);
    digitalWrite(RGB_LED_B_PIN, OUTPUT);
    ledOn();
    dht.begin();

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

void OutputModule::pumpOn(int reason) 
{
    pumpTimeOn = millis();
    pumpOnReason = reason;
    digitalWrite(PUMP_PIN, LOW);
}

int OutputModule::getOnReason() 
{
    return pumpOnReason;
}

void OutputModule::pumpOff() 
{
    digitalWrite(PUMP_PIN, HIGH);
    wateringTime = 0;
}

int OutputModule::getLeftTimeInSec() 
{
    if(pumpTimeOn + wateringTime < millis()){
        return -1;
    }
    return (pumpTimeOn + wateringTime - millis())/1000;
}

int OutputModule::pumpStatus() 
{
    return !digitalRead(PUMP_PIN);
}

void OutputModule::pumpOnForTimeSec(unsigned long duration, int reason) 
{

    wateringTime = duration*1000;
    pumpOn(reason);
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

void OutputModule::ledSetR(uint8_t v) 
{
    ledR = v;
    analogWrite(RGB_LED_R_PIN, ledR);
}

void OutputModule::ledSetG(uint8_t v) 
{
    ledG = v;
    analogWrite(RGB_LED_G_PIN, ledG);
}

void OutputModule::ledSetB(uint8_t v) 
{
    ledB = v;
    analogWrite(RGB_LED_B_PIN, ledB);
}


float OutputModule::readAirHumidity() 
{
      // read without samples.
    float newH = dht.readHumidity(); // TODO float value is present
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read humidityfrom DHT sensor!");
    }
    return newH;

}

float OutputModule::readSoilHumidity() 
{
    return (analogRead(SOIL_HUMIDITY_SENSOR)/900.0)*100.0;
}

float OutputModule::readAirTemp() 
{
    float newT = dht.readTemperature();
    if (isnan(newT)) {
      Serial.println("Failed to read temp from DHT sensor!");
    }

    return (float)((int)newT);
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
