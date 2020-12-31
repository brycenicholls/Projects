#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define OLED_CLOCK  15          // Pins for the OLED display
#define OLED_DATA   4
#define OLED_RESET  16

#define NUM_LEDS    10          // FastLED definitions
#define LED_PIN     5

CRGB g_LEDs[NUM_LEDS] = {0};    // Frame buffer for FastLED

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_lineHeight = 0;
int g_Brightness = 100;         // 0-255 LED brightness scale
int g_PowerLimit = 900;         // 900mW Power Limit


#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000/x)
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))  


// FractionalColor
//
// Returns a fraction of a color; abstracts the fadeToBlack away so that we can later
// do better color correction as needed

CRGB ColorFraction(const CRGB colorIn, float fraction)
{
  fraction = min(1.0f, fraction);
  return CRGB(colorIn).fadeToBlackBy(255 * (1.0f - fraction));
}



// DrawPixels
//
// Draw a sub-pixel precise amount of pixels starting at a floating point offset; for example
// you can draw 2.75 pixels starting a 5.5, and it will end at 8.25

void DrawPixels(float fPos, float count, CRGB color)
{
    // Figure out how much the first pixel will hold
    float availFirstPixel = 1.0f - (fPos - (long)(fPos));  // If we are starting at 2.25, there would be 0.75 avail here
    float amtFirstPixel = min(availFirstPixel, count);     // But of course we never draw more than we need
    float remaining = min(count, FastLED.size()-fPos);     // How many pixels remain after we draw the front header pixel
    int iPos = fPos;

    // Blend (add) in the color value of this first partial pixel ...and decrement the remaining pixel count by that same amount

    if (remaining > 0.0f)
    {
      FastLED.leds()[iPos++] += ColorFraction(color, amtFirstPixel); 
      remaining -= amtFirstPixel;                          
    }

    // Draw any full pixels and stop when we have a full pixel or less remainining

    while (remaining > 1.0f)                               // Final pixel can 'handle' up to 1.0 full pixels, so we draw anything more here
    {
      FastLED.leds()[iPos++] += color;                     // Draw them in one at aa time and update the remaining counts
      remaining--;
    }  

    // Draw tail pixel, up to a single full pixel

    if (remaining > 0.0f)  
    {                    
        FastLED.leds()[iPos] += ColorFraction(color, remaining);     
    }
}

#include "marquee.h"

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

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
  FastLED.setBrightness(g_Brightness);
  set_max_power_indicator_LED(LED_BUILTIN);                               // FastLED will light LED if power limiting
  FastLED.setMaxPowerInMilliWatts(g_PowerLimit);
}

void loop() 
{
  bool bLED = 0;
  
  while (true)
  {
    EVERY_N_MILLISECONDS(20)
    {
      fadeToBlackBy(g_LEDs, NUM_LEDS, 64);
      int cometSize = 15;
      int iPos = beatsin16(32, 0, NUM_LEDS-cometSize);
      byte hue = beatsin8(96);
      for (int i = iPos; i < iPos + cometSize; i++)
        g_LEDs[i] = CHSV(hue, 255, 255);
    }
    EVERY_N_MILLISECONDS(250)
    {
      g_OLED.clearBuffer();
      g_OLED.setCursor(0, g_lineHeight);
      g_OLED.print("Firelight");
      g_OLED.setCursor(0, g_lineHeight * 2);
      g_OLED.printf("LEDS:      %d", NUM_LEDS);
      g_OLED.setCursor(0, g_lineHeight * 3);
      g_OLED.printf("FPS:       %u", FastLED.getFPS());
      g_OLED.setCursor(0, g_lineHeight * 4);
      g_OLED.printf("POWER:     %u mW", calculate_unscaled_power_mW(g_LEDs, NUM_LEDS));
      g_OLED.setCursor(0, g_lineHeight * 5);
      g_OLED.printf("LUMENS:    %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
      g_OLED.sendBuffer();

  }
    FastLED.setBrightness(g_Brightness);                              // Set the brightness scale
    FastLED.delay(10);                                                // Show the delay

  }
}
