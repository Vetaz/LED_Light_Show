#include <FastLED.h>
#include <math.h>

#define LED_PIN             6
#define TOTAL_NUM_LEDS      253 // 253 for Sam's LED Strip; 60 for Jordi's.
#define LED_TYPE            WS2812
#define COLOR_ORDER         GRB
#define UPDATES_PER_SECOND  100 // how fast the LEDs change color/brightness per second.

int analogPin = A3; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int analogVal = 0;  // variable to store the value read
uint8_t overallBrightness = 100; // a 0-255 value for how much to scale all leds before writing them out
uint8_t prevNumLEDs = 0;

CRGB leds[TOTAL_NUM_LEDS];

void setup() {
  delay(3000); // Power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, TOTAL_NUM_LEDS);
  FastLED.setBrightness(overallBrightness);
  Serial.begin(9600);
}

/* Fill LEDs up to the numLEDs */ 
void fillLEDs(uint8_t saturation, uint8_t numLEDs)
{
  uint8_t hue;
  uint8_t brightness;
  
  // Making sure saturation is 0-255
  if (saturation < 0 || saturation > 255) {
    if (saturation < 0) {
      saturation = 0;
    } else {
      saturation = 255;
    }
  }
    // Making sure numLEDs is 0-253 (if voltage reads over 746, fill all LEDs)
  if (numLEDs < 0 || numLEDs > 253) {
    if (numLEDs < 0) {
      numLEDs = 0;
    } else {
      numLEDs = 253;
    }
  }
 
  for(int i = 0; i < numLEDs; i++) {
    hue = (uint8_t) ((float)i / TOTAL_NUM_LEDS * 250); // going up to hue value of 250 instead of the max of 255.
    brightness = (uint8_t) ((float)i / numLEDs * 55 + 200); // Brightness from 200 to 255.
    leds[i] = CHSV(hue, saturation, brightness);
    if (numLEDs < 48 && i % 6 == 0){ // Show filled LEDs every 6 LEDS
        FastLED.show();
    }
    else if (numLEDs < 96 && i % 8 == 0){ // Show filled LEDs every 8 LEDS
        FastLED.show();
    }
    else if (i % 12 == 0){ // Show filled LEDs every 12 LEDS
        FastLED.show();
    }
  }
}

/* Unfill LEDs down to the numLEDs */ 
void unfillLEDs(uint8_t prevNumLEDs, uint8_t numLEDs) { 
  for (int i = prevNumLEDs; i >= numLEDs; i--)
  {
    leds[i] = CRGB(0, 0, 0);
//    if (prevNumLEDs > 96 && i % 12 == 0){ // unfill every 6 LEDS
//        FastLED.show();
//    }
//    else 
    if (prevNumLEDs > 96 && i % 8 == 0){ // unfill every 8 LEDS
        FastLED.show();
    }
    else if (i % 12 == 0){ // unfill every 12 LEDS
        FastLED.show();
    }
  }
}

// code to help test for Jordi
//  static int digitalInput[1] = {1023}; //{100,200,300,450,300,100,500,700,800,1023,1023,800,300,200,100,400,500,700,800,500,400,600,300};
//  uint8_t index = 0;
// end of Jordi's code to help test.

void loop() {
  static uint8_t saturation = 255;
 
  analogVal = analogRead(analogPin);  // read the input pin, returns 0-1023 based on voltage 0v-5v

  // code to help test for Jordi commented out for you, Sam.
//  analogVal = digitalInput[index % 1];
//  index++;
  // end of Jordi's code to help test.

  // brightness =(uint8_t)(((float) analogVal / 1023) * 255); //brighter the higher the voltage, casting to float to perform math
  // hue = (uint8_t)(((float) analogVal / 1023) * 255); //different hue with differet voltage
  uint8_t numLEDs = (uint8_t)(((float) analogVal / 746) * TOTAL_NUM_LEDS); //the number of LEDS that should be on based on analog input

  if (prevNumLEDs < numLEDs) {
    fillLEDs(saturation, numLEDs);
  } else {
    unfillLEDs(prevNumLEDs, numLEDs);
  }
  
  prevNumLEDs = numLEDs;
}
