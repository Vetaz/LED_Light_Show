
// Sample Rate = 8000, sineWave has 32 samples which is 1 full wave 
// 1 period = SamplesInWave / SampleRate = 0.004 seconds
// Frequency = 1 / Period = 250 Hz
// Tone generated at 250Hz with an amplitude of .5.
static float sineWave[32] = {0.00000,0.09755,0.19134,0.27779,0.35355,0.41573,0.46194,0.49039,0.50000,0.49039,0.46194,0.41573,0.35355,0.27779,0.19134,0.09755,0.00000,-0.09755,-0.19134,-0.27779,-0.35355,-0.41573,-0.46194,-0.49039,-0.50000,-0.49039,-0.46194,-0.41573,-0.35355,-0.27779,-0.19134,-0.09755};
int sineWaveSize;
static int sampleRate = 8000;

static uint16_t frequencies[10] = {60, 90, 120, 240, 480, 960, 1250, 2500, 5000, 10000};
static uint8_t currentFreqNum = 0;

static uint8_t brightnesses[18] = {130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 210, 200, 190, 180, 170, 160, 150, 140};
static uint8_t currentBrightnessNum = 0;

static uint8_t currentLEDs = 0;

//TODO: volume changes num leds on...
// Sine wave: y = A sin (k (theta - b)) + c
// A = Amplitude, k = periodicity factor, b = left/right shift, c = up/down shift

// 60-250 HZ = base

#include <FastLED.h>
#include <math.h>

// Using 60 Hz - 10 kHz for range of colors
// hue = Log2(freq) - Log2(LOWER_FREQ) / (Log2(UPPER_FREQ) - Log2(LOWER_FREQ)) * 359
#define UPPER_FREQ  10000
#define LOWER_FREQ  60

#define LED_PIN     6
#define NUM_LEDS    60 // Change to 300 for Sam's LED Strip; 60 for Jordi's.
#define BRIGHTNESS  32 // It can be higher, but keep at 32 for now?
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100 // how fast the LEDs change color/brightness per second.

CRGB leds[NUM_LEDS];

void setup() { 
  delay(3000); // power-up safety delay
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS); 
  FastLED.setBrightness(BRIGHTNESS);

  sineWaveSize = sizeof(sineWave) / sizeof(float);
}

void FillLEDs(uint8_t hue, uint8_t brightness, uint8_t saturation)
{
  // Brightness from 0-255;
  // Saturation from 0-255;
  currentLEDs++;
  // Turn on a fraction of the LEDs if birghtness is not turned all the way up
  uint8_t adjNumLEDs = brightness / 255 * NUM_LEDS; 
  for(int i = 0; i < currentLEDs % NUM_LEDS; i++) {
    leds[i] = CHSV(hue, saturation, brightness);
  }
  for(int i = currentLEDs % NUM_LEDS; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, saturation, 0);
  }
}

/* Get the pitch frequency */
float getFreq() {
  currentFreqNum++;
  return frequencies[currentFreqNum % 11]; // Going through the frequencies each time this function is called.
}

/* Get the volume amplitude 
   returns a number from 0 to 255 for brightness */
uint8_t getBright(){
//  currentBrightnessNum++;
//  return brightnesses[currentBrightnessNum % 19]; // Going through the brightnesses each time this function is called.

  return (volt - 3.3) / (5 - 3.3) * 255;
}

void loop() { 
  static uint8_t hue = 0;
  static uint8_t saturation = 255;
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  float freq = getFreq();
  uint8_t brightness = getBright();

  // Changes color based on Frequency.
  hue = ((log(freq)/log(2)) - (log(LOWER_FREQ)/log(2))) / ((log(UPPER_FREQ)/log(2)) - (log(LOWER_FREQ)/log(2))) * 250; // For 60 Hz, Hue = 0; for 10 kHz, Hue = 250
  
  FillLEDs(hue, brightness, saturation);
  
  FastLED.show();
  delay(1000 / UPDATES_PER_SECOND);
}
