#include "application.h"

#include "mcc.h"

MCC mcc;

int ledPin1 = D7;
//int ledPin2 = D0;

// An UDP instance to let us send and receive packets over UDP
UDP Udp;

#include "modules/AdminModule.h"
AdminModule adminModule;
#include "modules/MotorModule.h"
MotorModule motorModule;

const char started_str[] = "STARTED";

void blink()
{
  //digitalWrite(ledPin2,  digitalRead(ledPin1));
  digitalWrite(ledPin1, 1-digitalRead(ledPin1));
}
Timer blinkTimer(500, blink);

void init()
{
  pinMode(ledPin1, OUTPUT);
  //pinMode(ledPin2, OUTPUT);
  Serial.begin(9600);

  blinkTimer.start();
}
STARTUP( init() );

void setup()
{
  Udp.begin(MCC_PORT);

  // Print your device IP Address via serial
  Serial.println(WiFi.localIP());

  motorModule.init();
  adminModule.report_status(started_str, sizeof(started_str)-1);
}


void loop()
{
  mcc.tick();
}

