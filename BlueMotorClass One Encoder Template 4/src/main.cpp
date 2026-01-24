#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"

BlueMotor motor;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;

long prevPos = 0;
unsigned long prevTime = 0;
const int TICKS_PER_REV = 1440; // adjust to your encoder

void setup() {
  Serial.begin(9600);
  delay(1000); // short delay to allow Serial to connect

  motor.setup();
  motor.reset();

  prevPos = motor.getPosition();
  prevTime = millis();

  Serial.println("Time(ms) | Position | Speed(RPM)");
}

void loop() {
  // --- Motor control ---
  if (buttonB.isPressed()) {
    motor.setEffort(300);
  } else if (buttonC.isPressed()) {
    motor.setEffort(-300);
  } else {
    motor.setEffort(0);
  }

  // --- Read current position and time ---
  long pos = motor.getPosition();
  unsigned long currTime = millis();

  // --- Calculate speed (RPM) ---
  float rpm = (pos - prevPos) / float(TICKS_PER_REV) * 60000.0 / (currTime - prevTime);

  // --- Print Time, Position, and Speed ---
  Serial.print(currTime);
  Serial.print(" | ");
  Serial.print(pos);
  Serial.print(" | ");
  Serial.println(rpm, 2); // 2 decimal places

  // --- Update previous values ---
  prevPos = pos;
  prevTime = currTime;

  delay(100); // adjust loop rate if needed
}
