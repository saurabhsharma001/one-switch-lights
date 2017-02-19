/*                                
  My Science Fair Project   
  The one switch Smart House
*/
#include <TM1637Display.h>  // 4-digit Diplay library

const int CLK = 6;          // Set the CLK pin connection to the display
const int DIO = 7;          // Set the DIO pin connection to the display

TM1637Display display(CLK, DIO);  // set up the 4-Digit Display.

int porchLight = 2;         // Prorch light should be conected to pin 2 on the arduino
int coachLights = 4;        // Coach lights should be conected to pin 4 on the arduino
int backyardLight = 8;      // Backyard light should be conected to pin 8 on the arduino
int switchPin = 12;         // Switch should be conected to pin 12 on the arduino
bool isPorchLightOn = false; // To track if the light is on or off
bool areCoachLightsOn = false; // To track if the light is on or off
bool isBackyardLightOn = false; // To track if the light is on or off

long timer = 0;             // 

void setup()               // It is used to initialize variables, pin modes, etc. The setup function will only run once.
{
  pinMode(porchLight, OUTPUT); // Telling the arduino that the Light pin is an output
  pinMode(coachLights, OUTPUT); // Telling the arduino that the Light pin is an output
  pinMode(backyardLight, OUTPUT); // Telling the arduino that the Light pin is an output
  pinMode(switchPin, INPUT); // Telling the arduino that Switch pin is an input 

  display.setBrightness(10); // Set brightness of LED display to 100%

  Serial.begin(9600);       //For debugging on serial port
}

void loop()                 // This function runs forever until it is on 
{
  while (digitalRead(switchPin) == HIGH) // Until Switch is high, the arduino stays in this loop
  {
    delay (100); // Delays the code for 100 milliseconds
    timer = timer + 100; 
    display.showNumberDec(timer);
    Serial.println(timer);
  }

  if (timer > 100 && timer <= 1000)
  {
    isPorchLightOn = !isPorchLightOn;
    digitalWrite(porchLight, isPorchLightOn);
    timer = 0;
  }

  if ( timer > 1000 && timer <= 3000)
  {
    areCoachLightsOn = !areCoachLightsOn;
    digitalWrite(coachLights, areCoachLightsOn);
    timer = 0;
  }

  if (timer > 3000)
  {
    isBackyardLightOn = !isBackyardLightOn;
    digitalWrite(backyardLight, isBackyardLightOn);
    timer = 0;
  }
  display.showNumberDec(0);
}
