#include <Arduino.h>

// Print Interval & Interrupt Interval
boolean interruptBool = false;

// IR Pin
const int irPin = 5;

// Photo Transistor Pin
const int photoPin = 14;

const int ledPin = 2;

// Interrupt Function
void ICACHE_RAM_ATTR photoISR()
{
  digitalWrite(ledPin, LOW);
  interruptBool = true;
}

void setup()
{
  // Start Serial Monitor
  Serial.begin(74880);

  // Create Initial Interrupt for Saving Data
  //attachInterrupt(0, photoISR, FALLING);

  pinMode(photoPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(photoPin), photoISR, FALLING);

  // Initial Intro Printout
  Serial.println();
  Serial.println("Welcome to Tacometer Reader.");
  Serial.println("To Get Past Values Input: VALUES");
  Serial.println("To Exit input: STOP");

  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, HIGH);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop()
{
  if ( interruptBool == true ){
    Serial.println("Interrupt Occured");
    digitalWrite(ledPin, HIGH);
    interruptBool = false;
  }
  else {
    Serial.println("All Groovy");
  }
}
