#include "Motors.h"
#include "Sensors.h"
#include "LEDs.h"
// #include "Floodfill.h"
#include "State.h"
#include "Maze.h"
#include "Encoder.h"
#include "pwm.h"
#include "Buzzer.h"

volatile int encoderValueLeft = 0;
volatile int encoderValueRight = 0;

Motors motors;
Sensors sensors(leftPT, frontPT, rightPT);

IntervalTimer sensorTimer;
IntervalTimer speedProfileTimer;
elapsedMillis wait;

void setup() {
  attachInterrupts();
  initializeTimers();
  initializeOnboardLED();
  initializeBuzzer();
  randomSeed(analogRead(0));  // Seeds using random analog noise on unconnected pin
  Serial.begin(9600);
  Serial1.print("Starting...\n");
  while (sensors.frontPTReading < 500) {  // Wait to enter loop
    blink(1);
  }
//  chirp();
  delay(2000);
  calibrateTargetValues();
  wait = 0;
}
void loop() {
  newNavigate();
}

void outputData(double data) {
  Serial.println(data);
}
void outputData(double left, double right) {
  Serial.print(left);
  Serial.print("/");
  Serial.println(right);
}

void countLeftEncoderA() {   // ++ if going forwards
  if (digitalRead(encoderLEFT_A)) {
    if (digitalRead(encoderLEFT_B)) { // If channel A leads B, CW
      encoderValueLeft--;
    }
    else {
      encoderValueLeft++;
    }
  }
  else {
    if (digitalRead(encoderLEFT_B)) {
      encoderValueLeft++;
    }
    else {
      encoderValueLeft--;
    }
  }
}

void countRightEncoderA() {  // ++ if going forwards
  if (digitalRead(encoderRIGHT_A)) {
    if (digitalRead(encoderRIGHT_B)) { // If channel A leads B, CW
      encoderValueRight--;
    }
    else {
      encoderValueRight++;
    }
  }
  else {
    if (digitalRead(encoderRIGHT_B)) { // If channel A leads B, CW
      encoderValueRight++;
    }
    else {
      encoderValueRight--;
    }
  }
}

void countLeftEncoderB() {
  if (digitalRead(encoderLEFT_B)) {
    if (digitalRead(encoderLEFT_A)) {
      encoderValueLeft++;
    }
    else {
      encoderValueLeft--;
    }
  }
  else {
    if (digitalRead(encoderLEFT_A)) {
      encoderValueLeft--;
    }
    else {
      encoderValueLeft++;
    }
  }
}

void countRightEncoderB() {
  if (digitalRead(encoderRIGHT_B)) {
    if (digitalRead(encoderRIGHT_A)) {
      encoderValueRight++;
    }
    else {
      encoderValueRight--;
    }
  }
  else {
    if (digitalRead(encoderRIGHT_A)) {
      encoderValueRight--;
    }
    else {
      encoderValueRight++;
    }
  }
}

void speedProfile() {
  getEncoderStatus();
  updateCurrentSpeed();
  calculateMotorPwm();
}

void readSensors() {
  sensors.leftPTReading = analogRead(leftPT);
  sensors.frontPTReading = analogRead(frontPT);
  sensors.rightPTReading = analogRead(rightPT);
  calculateSensorError();
}

void calibrateTargetValues() {
  resetSpeedProfile();
  targetRight = sensors.rightPTReading;
  targetLeft = sensors.leftPTReading;
  thresholdSide = (targetRight + targetLeft) / 10;
  turnRightEncoderTicks();
  targetFront = sensors.frontPTReading;
  thresholdFront = targetFront / 10;
  turnLeftEncoderTicks();
}

void initializeTimers() {
  sensorTimer.begin(readSensors, 5000);
  sensorTimer.priority(172);
  speedProfileTimer.begin(speedProfile, 5000);
  speedProfileTimer.priority(172);
}

void attachInterrupts() {
  attachInterrupt(encoderLEFT_A, countLeftEncoderA, CHANGE);
  attachInterrupt(encoderRIGHT_A, countRightEncoderA, CHANGE);
  attachInterrupt(encoderLEFT_B, countLeftEncoderB, CHANGE);
  attachInterrupt(encoderRIGHT_B, countRightEncoderB, CHANGE);
}
