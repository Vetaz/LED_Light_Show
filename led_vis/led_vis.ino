#include <FastLED.h>
#include <math.h>

#define LED_PIN             6       // Data pin for the LED strip
#define TOTAL_NUM_LEDS      300     // Change to how many LEDs you want to show
#define LED_TYPE            WS2812
#define COLOR_ORDER         GRB
#define UPDATES_PER_SECOND  100     // How fast the LEDs change color/brightness per second.
#define HIGHEST_ANALOG_INPUT_ALLOWED     746     // The cut-off voltage from 0-1023. 
                                    // The lower the number, the more LEDs are lit for a specific voltage.

int analogPin = A3; // Potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int analogVal = 0;  // A variable to store the value read
uint8_t overallBrightness = 100; // A 0-255 value for how much to scale all leds before writing them out
uint8_t prevNumLEDs = 0;


CRGB leds[TOTAL_NUM_LEDS];

void setup() {
  delay(3000); // 3 second power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, TOTAL_NUM_LEDS);
  FastLED.setBrightness(overallBrightness);
  Serial.begin(9600);
}

/* Fill LEDs up to the numLEDs */ 
void fillLEDs(uint8_t numLEDs)
{
  uint8_t hue;
  uint8_t brightness;
  
  for(int i = 0; i < numLEDs; i++) {
    hue = (uint8_t) ((float)i / TOTAL_NUM_LEDS * 250); // LED colors go from Red (0) to Pink (250)
    brightness = (uint8_t) ((float)i / numLEDs * 55 + 200); // Brightness from 200 to 255.
    leds[i] = CHSV(hue, 255, brightness);
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
    leds[i] = CRGB(0, 0, 0); // Turn off LED
    if (prevNumLEDs > 96 && i % 8 == 0){ // Unfill every 8 LEDS
        FastLED.show();
    }
    else if (i % 12 == 0){ // Unfill every 12 LEDS
        FastLED.show();
    }
  }
}

void loop() {
  analogVal = analogRead(analogPin);  // Read the input pin, returns 0-1023 based on voltage 0v-5v

  // The number of LEDS that should be on based on analog input
  uint8_t numLEDs = (uint8_t)(((float) analogVal / HIGHEST_ANALOG_INPUT_ALLOWED) * TOTAL_NUM_LEDS); 

  // Making sure numLEDs is 0 to TOTAL_NUM_LEDS (if voltage reads over HIGHEST_ANALOG_INPUT_ALLOWED, fill all LEDs)
  if (numLEDs < 0 || numLEDs > TOTAL_NUM_LEDS) {
    if (numLEDs < 0) {
      numLEDs = 0;
    } else {
      numLEDs = TOTAL_NUM_LEDS;
    }
  }

  if (prevNumLEDs < numLEDs) {
    fillLEDs(numLEDs);
  } else {
    unfillLEDs(prevNumLEDs, numLEDs);
  }
  
  prevNumLEDs = numLEDs;
}
