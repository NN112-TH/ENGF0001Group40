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
float stirPwm = 0;
float stirOutCount = 5000;

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

<<<<<<< Updated upstream
unsigned long printTime = 5000;
=======
//set initial condition
float setHeat = 303;
float setStir = 800;
float setpH = 5;

>>>>>>> Stashed changes

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

void stirOutput(float deltaPwm) 
{
<<<<<<< Updated upstream
    stirOutCount++;
    if (stirOutCount >= 5000)
    {
        float newPwm = stirPwm + deltaPwm;
        if (newPwm > 255)
            stirPwm = 255;
        else if (newPwm < 0)
            stirPwm = 0;
        else
            stirPwm = newPwm;
        analogWrite(stirrerPin, stirPwm);
        stirOutCount = 0;
=======
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
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        float p = pHSum / pHCount;
        pH = 0.00007 * pow(p,2) - 0.0212 * 250 + 3.8817;
=======
        pH = pHSum / pHCount;
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        
        Wire.beginTransmission(I2CADDR);
        Wire.write(0x06);
        Wire.write(0x00);
        Wire.write(0x00);
        Wire.write(0xFF);
        Wire.write(0xFF);
        Wire.endTransmission();
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        
        Wire.beginTransmission(I2CADDR);
        Wire.write(0x0A);
        Wire.write(0x00);
        Wire.write(0x00);
        Wire.write(0xFF);
        Wire.write(0xFF);
        Wire.endTransmission();
=======
>>>>>>> Stashed changes
    }
    
}

float pid(float input, float setpoint, float* prevTime, float* prevErr, float kP, float kI, float kD) 
{
    float now = millis();
    float elapsed = now - *prevTime;
    if (elapsed == 0)
        elapsed = 0.1;
    *prevTime = now;

    float error = setpoint - input;
    float cumErr = error * elapsed;
    float rateErr = (error - *prevErr) / elapsed;
    *prevErr = error;

    return kP * error + kI * cumErr + kD * rateErr;
}


void loop() 
{
    float pStir = 0.04;
    float iStir = 0.03;
    float dStir = 0;
  
    float setStir = 510;
  
    stirInput();
    float pidStir = pid(rpm, setStir, &prevStirTime, &prevStirErr, pStir, iStir, dStir);
    stirOutput(pidStir);
    
    float pHeat = 100;
    float iHeat = 100;
    float dHeat = 0.0;
    
    float pStir = 0.05;
    float iStir = 0.005;
    float dStir = 0.7;
    
    //processing command
    
    if(Serial.available()) {
      char val = Serial.read();
      //heat
      if(val == 'q')
      {
        setHeat += 0.5;
      }
      if(val == 'W')
      {
        setHeat -= 0.5;
      }
      if(val == 'e')
      {
        setHeat = 298;
      }
      if(val == 'r')
      {
        setHeat = 308;
      }
      //stir
      if(val == 'z')
      {
        setStir += 100;
      }
      if(val == 'x')
      {
        setStir -= 100;
      }
      if(val == 'c')
      {
        setStir = 1500;
      }
      if(val == 'w')
      {
        setStir = 500;
      }
      //pH
      if(val == 'a')
      {
        setpH += 0.5;
      }
      if(val == 's')
      {
        setpH -= 0.5;
      }
      if(val == 'd')
      {
        setpH = 7;
      }
      if(val == 'f')
      {
        setpH = 3;
      }
    }
  
<<<<<<< Updated upstream
    float setHeat = 300;
  
    heatInput();
    float pidHeat = pid(temp, setHeat, &prevHeatTime, &prevHeatErr, pHeat, iHeat, dHeat);
    heatOutput(pidHeat);
    
    float pPH = 2;
    float iPH = 0;
    float dPH = 0;
  
    float setPH = 6;
  
    pHInput();
    float pidPH = pid(pH, setPH-0.75, &prevPHTime, &prevPHErr, pPH, iPH, dPH);
    pHOutput(pidPH);
    
    float now = millis();
    if (now >= printTime)
    {
        Serial.print("RPM: ");
        Serial.println(rpm);
        Serial.print("Temp: ");
        Serial.println(temp);
        Serial.print("pH: ");
        Serial.println(pH);
        
        printTime = now + 5000;
    }
=======
    
  
    heatInput();
    stirInput();
    pHInput();
    
    float pHs =5.00;
    //send data to processing
    String temp_s = String(temp, 2);
    String rpm_s = String(rpm,0);
    String pH_s = String(pHs,2);
    Serial.println(temp_s + "," +pH_s +","+rpm_s+","+"66.0" );

    
    //wait for full integration
    float pidHeat = pid(temp, setHeat, &prevHeatTime, &prevHeatErr, pHeat, iHeat, dHeat);
    heatOutput(pidHeat);
    float pidStir = pid(rpm, setStir-30, &prevStirTime, &prevStirErr, pStir, iStir, dStir);
    stirOutput(pidStir);
    //delay
    delay(5);
>>>>>>> Stashed changes
}
