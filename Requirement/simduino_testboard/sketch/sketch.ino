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

float prev_stirring_time = 0;
float prev_stirring_error = 0;

float prev_rotation_time = 0;
int rpm_count = 0;
int rpm_sum = 0;
float rpm = 0;
float prev_stirrer_pwm = 0;

// The PCA9685 is connected to the default I2C connections. There is no need
// to set these explicitly.


void setup() {
  Serial.begin(9600);
  Serial.println("Hello World");

  pinMode(lightgatePin,  INPUT);
  pinMode(stirrerPin,    OUTPUT);
  pinMode(heaterPin,     OUTPUT);
  pinMode(thermistorPin, INPUT);
  pinMode(pHPin,         INPUT);
  
  attachInterrupt(digitalPinToInterrupt(lightgatePin), lightgate_change, FALLING);
}

void lightgate_change() {
  float current_time = millis();
  float time_period = current_time - prev_rotation_time;
  prev_rotation_time = current_time;
  rpm_sum += 60 * (1 / (2 * (time_period / 1000)));
  rpm_count++;
}

void stirring_input() {
  if (rpm_count >= 10) {
    rpm = rpm_sum / rpm_count;
    rpm_sum = 0;
    rpm_count = 0;
  }
}

void stirring_output(float pwm) {
  if (pwm > 0 && pwm < 255) {
    analogWrite(stirrerPin, pwm);
    prev_stirrer_pwm = pwm;
  }
  else if (pwm > 255) {
    analogWrite(stirrerPin, 255);
    prev_stirrer_pwm = 255;
  }
  else {
    analogWrite(stirrerPin, prev_stirrer_pwm - 2);
  }
}


float pid(float input, float setpoint, float* prev_time, float* prev_error, float k_p, float k_i, float k_d) {
  float current_time = millis();
  float elapsed_time = current_time - *prev_time;
  *prev_time = current_time;

  float error = setpoint - input;
  float cum_error = error * elapsed_time;
  float rate_error = (error - *prev_error) / elapsed_time;
  *prev_error = error;

  return k_p * error + k_i * cum_error + k_d * rate_error;
}

void loop() {
  float p_stirring = 0.05;
  float i_stirring = 0.005;
  float d_stirring = 0.7;
  
  float setpoint_stirring = 1000;
  
  stirring_input();
  Serial.println(rpm);
  float pid_stirring = pid(rpm, setpoint_stirring, &prev_stirring_time, &prev_stirring_error, p_stirring, i_stirring, d_stirring);
  stirring_output(pid_stirring);
}
