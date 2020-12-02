/*
 sketch.ino

 Stephen Hailes, Oct 2020

 This is a template sketch for the testboard simduino
  
 */

#include <stdio.h>
#include <string.h>

// These define which pins are connected to what device on the virtual bioreactor
//
const byte lightgatePin  = 2;
const byte stirrerPin    = 5;
const byte heaterPin     = 6;
const byte thermistorPin = A0;
const byte pHPin         = A1;

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

unsigned long printTime = 5000;

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
    
    float pHeat = 20;
    float iHeat = 0.0;
    float dHeat = 0.0;
  
    float setHeat = 298;
  
    heatInput();
    Serial.println(temp);
    float pidHeat = pid(temp, setHeat, &prevHeatTime, &prevHeatErr, pHeat, iHeat, dHeat);
    heatOutput(pidHeat);
}
