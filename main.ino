#include <TimerOne.h>
#include "Motors.h"
#include "Sensors.h"
#include "LEDs.h"
#include "PID.h"
// #include "Floodfill.h"
#include "State.h"
#include "Maze.h"
// #include "stm32f4xx.h"
// #include "delay.h"

volatile int encoderValueLeft = 0;
volatile int encoderValueRight = 0;
int targetRight;
int targetLeft;

void checkIfTooClose();
bool isTooClose();

PID PID(.33, 0, 0);
Motors motors;
Sensors sensors(leftPT, frontPT, rightPT);

void setup() {
  initializeBuzzer();
  initializeOnboardLED();
  Timer1.initialize(500);
  Timer1.start();
  randomSeed(analogRead(0));  // Seeds using random analog noise on unconnected pin
  Serial1.begin(9600);
  Timer1.attachInterrupt(readSensors);
  attachInterrupt(encoderLEFT_A, countLeft, RISING);
  attachInterrupt(encoderRIGHT_A, countRight, RISING);
  Serial1.print("Starting...\n");
  while (sensors.frontPTReading < 500) {  // Wait to enter loop
    blink(1);
  }
  delay(2000);
  targetRight = analogRead(rightPT);
  targetLeft = analogRead(leftPT);
}

void loop() {
  navigate();
}

void countLeft() {
//  if (digitalRead(encoderLEFT_B) == HIGH) { // If channel A leads B, CW
//    encoderValueLeft--;
//  }
//  else {
    encoderValueLeft++;
//  }
}

void countRight() {
//  if (digitalRead(encoderRIGHT_B) == HIGH) { // If channel A leads B, CW
    encoderValueRight++;
//  }
//  else {
//    encoderValueRight--;
//  }
}

void readSensors() {
  sensors.leftPTReading = analogRead(leftPT);
  sensors.frontPTReading = analogRead(frontPT);
  sensors.rightPTReading = analogRead(rightPT);
}
