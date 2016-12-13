#include "application.h"

int ledPin1 = D7;
int ledPin2 = D0;

void setup()
{
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  delay(250);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, HIGH);
  delay(250);
}

