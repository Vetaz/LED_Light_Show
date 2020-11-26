#include <FastLED.h>
#include <math.h>

#define LED_PIN     6
#define TOTAL_NUM_LEDS    60 // Change to 300 for Sam's LED Strip; 60 for Jordi's.
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100 // how fast the LEDs change color/brightness per second.
int analogPin = A3; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int analogVal = 0;  // variable to store the value read
uint8_t brightness = 32; //at 32 just to start out

CRGB leds[TOTAL_NUM_LEDS];

void setup() {
  delay(3000); // power-up safety delay
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, TOTAL_NUM_LEDS);
  FastLED.setBrightness(brightness);

}

void FillLEDs(uint8_t hue, uint8_t brightness, uint8_t saturation, uint8_t numLEDs)
{
  // Brightness from 0-255;
  // Saturation from 0-255;
  // Checks on brightness and Saturation that they are in range.
  if (brightness < 0 || brightness > 255) {
    if (brightness < 0) {
      brightness = 0;
    } else {
      brightness = 255;
    }
  }
  if (saturation < 0 || saturation > 255) {
    if (saturation < 0) {
      saturation = 0;
    } else {
      saturation = 255;
    }
  }
  fill_solid(leds, TOTAL_NUM_LEDS, CRGB(0,0,0)); // Makes all the LEDS turn off in between fillings.
  for(int i = 0; i < numLEDs; i++) {
    leds[i] = CHSV(hue, saturation, brightness);
  }
}

// code to help test for Jordi
static int digitalInput[23] = {100,200,300,450,300,100,500,700,800,1020,1020,800,300,200,100,400,500,700,800,500,400,600,300};
uint8_t index = 0;
// end of Jordi's code to help test.

void loop() {
  uint8_t hue = 0;
  static uint8_t saturation = 255;

  analogVal = analogRead(analogPin);  // read the input pin, returns 0-1023 based on voltage 0v-5v
  
  // code to help test for Jordi commented out for you, Sam.
//   analogVal = digitalInput[index % 23];
//   index = 1 + index;
//   Serial.println(analogVal);
  // end of Jordi's code to help test.
  
  brightness = (uint8_t)(((float) analogVal / 1023) * 255); //brighter the higher the voltage, casting to float to perform math
  hue = (uint8_t)(((float) analogVal / 1023) * 255); //different hue with differet voltage
  uint8_t numLEDs = (uint8_t)(((float) analogVal / 1023) * TOTAL_NUM_LEDS); //the number of LEDS that should be on based on analog input
 
  FillLEDs(hue, brightness, saturation, numLEDs);
 
  FastLED.show();
  delay(1000 / UPDATES_PER_SECOND);
}
