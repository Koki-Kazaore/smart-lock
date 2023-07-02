#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

#define FINGERPRINT_RX_PIN 18
#define FINGERPRINT_TX_PIN 19

#define LED_PIN 4

SoftwareSerial fingerSerial(FINGERPRINT_RX_PIN, FINGERPRINT_TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

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
    digitalWrite(LED_PIN, HIGH);  // LEDを点灯させる処理
    delay(2000);  // 2秒間LEDを点灯させる
    digitalWrite(LED_PIN, LOW);  // LEDを消灯させる処理
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