#include<Arduino.h>
#define FASTLED_INTERNAL
#include<FastLED.h>

#define NUM_COLORS 5
static const CRGB TwinkleColors [NUM_COLORS] = 
{
    CRGB::Red,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Green,
    CRGB::Orange
};

void DrawTwinkle()
{
    FastLED.clear(false);

    for (int i=0; i< NUM_LEDS / 4; i++)
    {
        g_LEDS[random(NUM_LEDS)] = TwinkleColors[random(0, NUM_COLORS)];
        FastLED.show(g_brightness);
        delay(200);
    }
}

void DrawTwinkle2()
{
    static int passCount = 0;
    if (passCount++ == NUM_LEDS)
    {
        passCount = 0;
        FastLED.clear(false);
    }
    g_LEDS[random(NUM_LEDS)] = TwinkleColors[random(0, NUM_COLORS)];
      
}