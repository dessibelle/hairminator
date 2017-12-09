#include <Adafruit_NeoPixel.h>

#define BUZZER_PIN          9

#define LED_TRIGGER_PIN     2
#define SFX_TRIGGER_PIN     3

#define STRIP_PIN           6
#define NUM_LEDS            12

#define PI                  3.1415926535897932384626433832795
#define HALF_PI             1.5707963267948966192313216916398
#define TWO_PI              6.283185307179586476925286766559
#define DEG_TO_RAD          0.017453292519943295769236907684886
#define RAD_TO_DEG          57.295779513082320876798154814105

#define LOOP_DELAY          25
#define BUZZ_TIME           15

#define MIN_FREQ            50
#define MAX_FREQ            800
#define FREQ_FACTOR         0.7
#define BASE_BRIGHTNESS     0.5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STRIP_PIN, NEO_GRB + NEO_KHZ800);


uint32_t i = 12; // Start at twelwe for simplicity, which lets us clear "previous" pixel right away

void setup() {
  pinMode(LED_TRIGGER_PIN, INPUT);
  pinMode(SFX_TRIGGER_PIN, INPUT);
  pinMode (BUZZER_PIN, OUTPUT) ;// set the digital IO pin mode, OUTPUT out of Wen
  strip.begin();

}

bool sholdBurnLeds() {
  return digitalRead(LED_TRIGGER_PIN) != LOW;
}

bool shouldPlaySound() {
 return digitalRead(SFX_TRIGGER_PIN) != LOW;
}

void loop() {
  uint8_t numPixels = 6;

  if (sholdBurnLeds()) {
    for (uint8_t p = 0; p < numPixels; p++) {
      float brightness = BASE_BRIGHTNESS * (numPixels - p) / (float)numPixels;

      strip.setPixelColor(getPixel(i - p), Wheel((i + p * (360 / 12)) % 255, brightness));
      strip.setPixelColor(getPixel(i - p - numPixels), 0); // Clear previous values "behind"
    }
  } else {
    // Clear all values
    for (uint8_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, 0);
    }
  }
  strip.show();

  if (shouldPlaySound()) {
    float frequencySin = sin((round(i * FREQ_FACTOR) % 360) * DEG_TO_RAD);
    int frequency = MIN_FREQ + (frequencySin + 1) * 0.5 * MAX_FREQ;
    tone(BUZZER_PIN, frequency, BUZZ_TIME);
  }

  delay(LOOP_DELAY);

  i++;
}

uint16_t getPixel(uint32_t i) {
  uint16_t maxValue = strip.numPixels();
  return i % maxValue;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, float brightness) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(brightness * (255 - WheelPos * 3), 0, brightness * WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, brightness * WheelPos * 3, brightness * (255 - WheelPos * 3));
  }
  WheelPos -= 170;
  return strip.Color(brightness * WheelPos * 3, brightness * (255 - WheelPos * 3), 0);
}

