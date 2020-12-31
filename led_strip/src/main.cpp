#include <Arduino.h>            // Arduino Framework
#include <U8g2lib.h>     
#define FASTLED_INTERNAL
#include <FastLED.h>
       // For text on the little on-chip OLED


#define OLED_CLOCK  15          // Pins for the OLED display
#define OLED_DATA   4
#define OLED_RESET  16

#define NUM_LEDS 60
#define LED_PIN 5

CRGB g_LEDS[NUM_LEDS] = {0};

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_lineHeight = 0;
int g_brightness = 200;   // Brightness scale

// Custom Libraries
#include<marquee.h>
#include<twinkle.h>
#include<comet.h>
// FramesPerSecond
//
// Tracks a weighted average to smooth out the values that it calcs as the simple reciprocal
// of the amount of time taken specified by the caller.  So 1/3 of a second is 3 fps, and it
// will take up to 10 frames or so to stabilize on that value.

double FramesPerSecond(double seconds)
{
  static double framesPerSecond; 
  framesPerSecond = (framesPerSecond * .9) + (1.0 / seconds * .1);
  return framesPerSecond;
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");

  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();        // Descent is a negative number so we add it to the total

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDS, NUM_LEDS);     // Add LED strip to FastLED library
  FastLED.setBrightness(g_brightness);                 // Set LED brightness


}

void loop() 
{
  bool bLED = 0;
  double fps = 0;

// Rainbow effect vars
  uint8_t initialHue = 0;        // Start position of the rainbow
  const uint8_t deltaHue = 16;    // Speed at which the colours change 
  const uint8_t hueDensity = 4;  // How tight the rainbow is going to be spread across the number of LEDS


  for (;;)
  {
    bLED = !bLED;
    digitalWrite(LED_BUILTIN, bLED);

    double dStart = millis() / 1000.0;                 // Display a frame and calc how long it takes
// OLED code
    g_OLED.clearBuffer();
    g_OLED.setCursor(0, g_lineHeight);
    g_OLED.print("Firelight");
    g_OLED.setCursor(0, g_lineHeight * 2);
    g_OLED.printf("LEDS:      %d", NUM_LEDS);
    g_OLED.setCursor(0, g_lineHeight * 3);
    g_OLED.printf("FPS:       %u", FastLED.getFPS());
    g_OLED.setCursor(0, g_lineHeight * 4);
//  g_OLED.printf("POWER:     %u mW", calculate_unscaled_power_mW(g_LEDs, NUM_LEDS));
    g_OLED.setCursor(0, g_lineHeight * 5);
//  g_OLED.printf("LUMENS:    %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
    g_OLED.sendBuffer();

// Handle LEDs
      
//    fill_rainbow(g_LEDS, NUM_LEDS, initialHue += hueDensity, deltaHue);
    DrawComet();

    FastLED.setBrightness(g_brightness);
    FastLED.show();

    double dEnd = millis() / 1000.0;
    fps = FramesPerSecond(dEnd - dStart);
  }
}