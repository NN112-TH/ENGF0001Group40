/*
 sketch.ino

 Stephen Hailes, Oct 2020

 This is a template sketch for the testboard simduino
  
 */

#include <stdio.h>
#include <string.h>
#include <Wire.h>

// These define which pins are connected to what device on the virtual bioreactor
//
const byte lightgatePin  = 2;
const byte stirrerPin    = 5;
const byte heaterPin     = 6;
const byte thermistorPin = A0;
const byte pHPin         = A1;
const byte I2CADDR = 0x40;

float prevStirTime = 0;
float prevStirErr = 0;

float prevRotTime = 0;
int rpmCount = 0;
int rpmSum = 0;
float rpm = 0;
float prevStirPwm = 0;

float prevHeatTime = 0;
float prevHeatErr = 0;

int tempCount = 0;
int tempSum = 0;
float temp = 0;

float prevPHTime = 0;
float prevPHErr = 0;

int pHCount = 0;
int pHSum  = 0;
float pH = 0;

// The PCA9685 is connected to the default I2C connections. There is no need
// to set these explicitly.

void setup() 
{
    Serial.begin(9600);
    Serial.println("Hello World");

    pinMode(lightgatePin,  INPUT);
    pinMode(stirrerPin,    OUTPUT);
    pinMode(heaterPin,     OUTPUT);
    pinMode(thermistorPin, INPUT);
    pinMode(pHPin,         INPUT);
  
    attachInterrupt(digitalPinToInterrupt(lightgatePin), lightgateChange, FALLING);
    
    Wire.beginTransmission(I2CADDR);
    Wire.write(0x00);
    Wire.write(0x21);
    Wire.endTransmission();
}

void lightgateChange() 
{
    float now = millis();
    float period = now - prevRotTime;
    prevRotTime = now;
    rpmSum += 60 * (1 / (2 * (period / 1000)));
    rpmCount++;
}

void stirInput() 
{
    if (rpmCount >= 15) 
    {
        rpm = rpmSum / rpmCount;
        rpmSum = 0;
        rpmCount = 0;
    }
}

void stirOutput(float pwm) 
{
    if (pwm > 0 && pwm < 255) 
    {
        analogWrite(stirrerPin, pwm);
        prevStirPwm = pwm;
    }
    else if (pwm > 255) 
    {
        analogWrite(stirrerPin, 255);
        prevStirPwm = 255;
    }
    else 
    {
        analogWrite(stirrerPin, prevStirPwm);
    }
}

void heatInput()
{
    tempCount++;
    tempSum += analogRead(thermistorPin);
    
    if (tempCount >= 30)
    {
        temp = (-0.0919 * (tempSum / tempCount)) + 345.14;
        tempCount = 0;
        tempSum = 0;
    }
}

void heatOutput(float pwm)
{
    if (pwm > 0 && pwm < 255) 
        analogWrite(heaterPin, pwm);
      
    else if (pwm > 255) 
        analogWrite(heaterPin, 255);
      
    else 
        analogWrite(heaterPin, 0);
}

void pHInput()
{
    pHCount++;
    pHSum += analogRead(pHPin);
    
    if (pHCount >= 30)
    {
        pH = pHSum / pHCount;
        pHCount = 0;
        pHSum = 0;
    }
}

void pHOutput(int on)
{
    if (on >= 0)
    {
        if (on > 4095)
        {
            on = 4095;
        }
        byte onLow = lowByte(on);
        
        int off = 4095 - on;
        byte offLow = lowByte(off);
        
        Wire.beginTransmission(I2CADDR);
        Wire.write(0x0A);
        Wire.write(onLow);
        Wire.write(on - onLow);
        Wire.write(offLow);
        Wire.write(off - offLow);
        Wire.endTransmission();
    }
    else
    {
        on = abs(on);
        if (on > 4095)
        {
            on = 4095;
        }
        byte onLow = lowByte(on);
        
        int off = 4095 - on;
        byte offLow = lowByte(off);
        
        Wire.beginTransmission(I2CADDR);
        Wire.write(0x06);
        Wire.write(onLow);
        Wire.write(on - onLow);
        Wire.write(offLow);
        Wire.write(off - offLow);
        Wire.endTransmission();
    }
    
}

float pid(float input, float setpoint, float* prevTime, float* prevErr, float kP, float kI, float kD) 
{
    float now = millis();
    float elapsed = now - *prevTime;
    *prevTime = now;

    float error = setpoint - input;
    float cumErr = error * elapsed;
    float rateErr = (error - *prevErr) / elapsed;
    *prevErr = error;

    return kP * error + kI * cumErr + kD * rateErr;
}

void loop() 
{
    /*float pStir = 0.05;
    float iStir = 0.005;
    float dStir = 0.7;
  
    float setStir = 800;
  
    stirInput();
    Serial.println(rpm);
    float pidStir = pid(rpm, setStir-30, &prevStirTime, &prevStirErr, pStir, iStir, dStir);
    stirOutput(pidStir);*/
    
    /*float pHeat = 20;
    float iHeat = 0.0;
    float dHeat = 0.0;
  
    float setHeat = 310;
  
    heatInput();
    Serial.println(temp);
    float pidHeat = pid(temp, setHeat, &prevHeatTime, &prevHeatErr, pHeat, iHeat, dHeat);
    heatOutput(pidHeat);*/
    
    pHInput();
    Serial.println(pH);
    
    pHOutput(-10);
}
