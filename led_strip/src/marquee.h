// Draw a marquee

#include<Arduino.h>
#define FASTLED_INTERNAL
#include<FastLED.h>

void DrawMarquee()
{
    static byte j = HUE_BLUE;
    j +=4;
    byte k = j;

    // The below is roughly likr the FastLED fill_rainbow function (g_LEDS, NUM_LEDS, j, 8)

    CRGB c;
    for (int i = 0; i < NUM_LEDS; i++)
        g_LEDS[i] = c.setHue(k+=8);

    static int scroll = 0;
    scroll++;

    for (int i=scroll % 5; i < NUM_LEDS - 1; i +=5)
        g_LEDS[i] = CRGB::Black;

    delay(50);
}