/*
  My Science Fair Project
  The Smart Home
*/
#include <TM1637Display.h>  // 4-digit Display library
#include <IRremote.h> //IR Sensor library
#include <Servo.h>  // Servo Motor library

const int CLK = 6;          // Set the CLK pin connection to the display
const int DIO = 7;          // Set the DIO pin connection to the display
TM1637Display display(CLK, DIO);  // set up the 4-Digit Display.

long unsigned int lowIn;
long unsigned int pause = 5000;
boolean lockLow = true;
boolean takeLowTime;

int porchLight = 2;         // Porch light should be connected to pin 2 on the arduino
int coachLights = 4;        // Coach lights should be connected to pin 4 on the arduino
int backyardLight = 8;      // Backyard light should be connected to pin 8 on the arduino
int floodLight = 9;
int switchPin = 12;         // Switch should be conected to pin 12 on the arduino
bool isPorchLightOn = false; // To track if the light is on or off
bool areCoachLightsOn = false; // To track if the light is on or off
bool isBackyardLightOn = false; // To track if the light is on or off
int pirPin = 11;

int servoMotor = 13;
Servo Servo1;

int RECV_PIN = 10;           //IR sensor should be conected to pin 10 on the arduino
long ONE = 16724175;
long TWO = 16718055;
long THREE = 16743045;
long FOUR = 16716015;
long FIVE = 16726215;
IRrecv irrecv(RECV_PIN);
decode_results results;

long timer = 0;

String passwordValue1 = "";
String passwordValue2 = "";
String passwordValue3 = "";
String passwordValue4 = "";
String correctPassword = "2413";
String inputPassword = "";
int wrongPasswordLight = 3;
int rightPasswordLight = 5;

void setup()               // It is used to initialize variables, pin modes, etc. The setup function will only run once.
{
  pinMode(porchLight, OUTPUT); // Telling the arduino that the Light pin is an output
  pinMode(coachLights, OUTPUT); // Telling the arduino that the Light pin is an output
  pinMode(backyardLight, OUTPUT); // Telling the arduino that the Light pin is an output
  pinMode(floodLight, OUTPUT);  // Telling the arduino that the Light pin is an output
  pinMode(wrongPasswordLight, OUTPUT); // Telling the arduino that the Light pin is an output
  pinMode(rightPasswordLight, OUTPUT);  // Telling the arduino that the Light pin is an output
  pinMode(switchPin, INPUT); // Telling the arduino that Switch pin is an input
  pinMode(pirPin, INPUT);   // Telling the arduino that the motion sensor is an input
  Servo1.attach(servoMotor);
  display.setBrightness(10); // Set brightness of LED display to 100%
  digitalWrite(pirPin, LOW);

  Serial.begin(9600);       //For debugging on serial port
  Serial.println("Enabling IRin");
  irrecv.enableIRIn();          // Start the receiver
}

void loop()                 // This function runs forever until it is on
{
  while (digitalRead(switchPin) == HIGH) {
    delay (100);                        // Delay for 100 miliseconds
    timer = timer + 100;
    display.showNumberDec(timer);
    Serial.println(timer);
  }
  if (timer > 100 && timer <= 1000) {
    isPorchLightOn = !isPorchLightOn;
    digitalWrite(porchLight, isPorchLightOn);
    timer = 0;
  }
  if ( timer > 1000 && timer <= 3000) {
    areCoachLightsOn = !areCoachLightsOn;
    digitalWrite(coachLights, areCoachLightsOn);
    timer = 0;
  }
  if (timer > 3000) {
    isBackyardLightOn = !isBackyardLightOn;
    digitalWrite(backyardLight, isBackyardLightOn);
    timer = 0;
  }
  if (inputPassword.length() == "") {
    display.showNumberDec(0);
  }
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    if (results.value == ONE) {
      isPorchLightOn = !isPorchLightOn;
      digitalWrite(porchLight, isPorchLightOn);
    }
    if (results.value == TWO) {
      areCoachLightsOn = !areCoachLightsOn;
      digitalWrite(coachLights, areCoachLightsOn);
    }
    if (results.value == THREE) {
      isBackyardLightOn = !isBackyardLightOn;
      digitalWrite(backyardLight, isBackyardLightOn);
    }
    if (results.value == FOUR) {
      isPorchLightOn = true;
      digitalWrite(porchLight, isPorchLightOn);
      areCoachLightsOn = true;
      digitalWrite(coachLights, areCoachLightsOn);
      isBackyardLightOn = true;
      digitalWrite(backyardLight, isBackyardLightOn);
    }
    if (results.value == FIVE) {
      isPorchLightOn = false;
      digitalWrite(porchLight, isPorchLightOn);
      areCoachLightsOn = false;
      digitalWrite(coachLights, areCoachLightsOn);
      isBackyardLightOn = false;
      digitalWrite(backyardLight, isBackyardLightOn);
    }
  }
  irrecv.resume(); // Receive the next value

  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(floodLight, HIGH);   //the led visualizes the sensors output pin state
    if (lockLow) {
      lockLow = false;
    }
    takeLowTime = true;
  }
  if (digitalRead(pirPin) == LOW) {
    digitalWrite(floodLight, LOW);
    if (takeLowTime) {
      lowIn = millis();
      takeLowTime = false;
    }
    if (!lockLow && millis() - lowIn > pause) {
      lockLow = true;
    }
  }

  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  int sensorValue3 = analogRead(A2);
  int sensorValue4 = analogRead(A3);

  if (sensorValue1 > 400) {
    Serial.println(1);
    passwordValue1 = "1";
    inputPassword = inputPassword + passwordValue1;
    Serial.println(inputPassword);
  }
  if (sensorValue2 > 400) {
    Serial.println(2);
    passwordValue2 = "2";
    inputPassword = inputPassword + passwordValue2;
  }
  if (sensorValue3 > 400) {
    Serial.println(3);
    passwordValue3 = "3";
    inputPassword = inputPassword + passwordValue3;
    Serial.println(inputPassword);
  }
  if (sensorValue4 > 400) {
    Serial.println(4);
    passwordValue4 = "4";
    inputPassword = inputPassword + passwordValue4;
    Serial.println(inputPassword);
  }
  display.showNumberDec(inputPassword.toInt());
  if (correctPassword == inputPassword) {
    Serial.println("correct password");
    inputPassword = "";
    digitalWrite(rightPasswordLight, HIGH);
    Servo1.write(90);
    delay(3000);
    digitalWrite(rightPasswordLight, LOW);
    Servo1.write(0);
  }
  if (inputPassword.length() >= 4) {
    inputPassword = "";
    digitalWrite(wrongPasswordLight, HIGH);
    delay(2000);
    digitalWrite(wrongPasswordLight, LOW);
  }
  delay(200);
}
