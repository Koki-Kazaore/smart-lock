/*
This is a basic program to check if the servo motor works.
*/

#include <Servo.h>

// Servo motor signal pin
#define SERVO_PIN 4

Servo servo;

void setup()
{
    // connect to signal pin of servo motor
    servo.attach(SERVO_PIN);  
}

void loop()
{
  // Move the servo motor to 90 degrees
  servo.write(90);
  // wait 2 seconds
  delay(2000);

  // Move the servo motor to 180 degrees
  servo.write(180);
  // wait 2 seconds
  delay(2000);
}