/*
This is a program that rotates the servo motor when the fingerprint authetication is successful.
*/

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define FINGERPRINT_RX_PIN 18
#define FINGERPRINT_TX_PIN 19

// Servo motor signal pin
#define SERVO_PIN 4

SoftwareSerial fingerSerial(FINGERPRINT_RX_PIN, FINGERPRINT_TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);
Servo servo;

void setup()
{
  Serial.begin(57600);
  fingerSerial.begin(57600);

  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }

  // connect to signal pin of servo motor
  servo.attach(SERVO_PIN);
}

void loop()
{
  getFingerprintID();
  delay(500);
}

void getFingerprintID()
{
  uint8_t p = finger.getImage();
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return;
    default:
      Serial.println("Unknown error");
      return;
  }

  p = finger.image2Tz();
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return;
    default:
      Serial.println("Unknown error");
      return;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Finger found!");
    // processing to rotate the servo motor
    rotateServo();
    return;
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return;
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    Serial.println("No match found");
    return;
  }
  else
  {
    Serial.println("Unknown error");
    return;
  }
}

void rotateServo()
{
    // Move the servo motor to 180 degrees(open)
    servo.write(180);
    // wait 2 seconds
    delay(10000);

    // Move the servo motor to 90 degrees(close)
    servo.write(90);
    // wait 2 seconds
    delay(2000);
}