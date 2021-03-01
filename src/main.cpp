#include <Arduino.h>
#include <Servo.h>

// Print Interval & Interrupt Interval
boolean interruptBool = false;

// IR Pin
const int irPin = 5;

// Photo Transistor Pin
const int photoPin = 14;

// Servo Pin & Object
const int servoPin = 12;
Servo servo1;
int REV = 0;
int rpm = 0;
int previousTime = 0;
bool setServoSpeed = true;
const int rpmInterval = 1000;

// LED Pin (used during interrupt)
const int ledPin = 2;

// CSV String
char csvData[20480] = "millis,rpm\n";

// Speed Function
void setSpeed(int speed)
{
  //Sets servo positions to different speeds
  int angle = map(speed, 0, 100, 0, 180); 
  servo1.write(angle);
}

// Interrupt Function
void ICACHE_RAM_ATTR photoISR()
{
  REV++;
  digitalWrite(ledPin, LOW);
  interruptBool = true;
}

void setup()
{
  // Start Serial Monitor
  Serial.begin(74880);

  // Create Initial Interrupt for Saving Data
  pinMode(photoPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(photoPin), photoISR, FALLING);

  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, HIGH);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  //pinMode(servoPin, OUTPUT);

  servo1.attach(servoPin);
  setSpeed(0);
  previousTime = millis();

  // Initial Intro Printout
  Serial.println();
  Serial.println("Welcome to Tacometer Reader.");
  Serial.println("To Get Past Values Input: VALUES");
  Serial.println("To Exit input: STOP");
}

void loop()
{

  if (Serial.available() > 0)
  {
    // read the incoming byte:
    String incoming_string = Serial.readString();

    // Add Check for Exit Code
    char incoming_string_buf[incoming_string.length() + 1];
    incoming_string.toCharArray(incoming_string_buf, incoming_string.length() + 1);
    if (strcmp(incoming_string_buf, "STOP") == 0)
    {
      Serial.println("Exiting...");
      exit(0);
    }
    else if (strcmp(incoming_string_buf, "VALUES") == 0)
    {
      Serial.println("VALUES...");
      Serial.print(csvData);
    }
  }

  if (interruptBool == true)
  {
    digitalWrite(ledPin, HIGH);
    interruptBool = false;
  }

  int currentTime = millis();
  if (currentTime - previousTime > rpmInterval)
  {
    Serial.println(REV);
    Serial.println(currentTime - previousTime);
    rpm = 60 * 1000 / (currentTime - previousTime) * REV;
    REV = 0;
    previousTime = millis();

    Serial.print("RPM: ");
    Serial.println(rpm);

    char tmp[22];
    // Save the Data in CSV Format

    sprintf(tmp, "%i,%i\n", millis(), rpm);

    strcat(csvData, tmp);
  }

  if (setServoSpeed)
  {
    Serial.println("Waiting 3 seconds to start");
    delay(3000);
    Serial.println("Setting Servo Speed");
    setServoSpeed = false;
    // Start Servo
    int speed;
    for (speed = 0; speed <= 2; speed += 1)
    { //Cycles speed up to 70% power for 1 second

      setSpeed(speed); //Creates variable for speed to be used in in for loop
      delay(1000);
    }
  }
}
