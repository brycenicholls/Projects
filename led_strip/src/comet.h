#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDS[];

void DrawComet()
{
    const byte fadeAmt = 128;  // Fraction of 256 to fade a LED if it is selected for fading
    const int cometSize = 3;   // Size of comet in LEDS
    const int deltaHue  = 4;   // How far to step through the cycling hue for each draw call

    static byte hue = HUE_RED;  // Current colour
    static int iDirection = 1;  // Current direction (+1 or -1)
    static int iPos = 0;        // Current position on the strip

    hue += deltaHue;            // Update the comet colour

    iPos += iDirection;         // Update the comet position
    if (iPos == (NUM_LEDS - cometSize) || iPos == 0)
        iDirection *= -1;
    
    for (int i = 0; i < cometSize; i++)
        g_LEDS[iPos + i].setHue(hue);
    
    // Randomly fade the LEDs
    for (int j = 0; j < NUM_LEDS; j++)
        if (random(10) > 5)
            g_LEDS[j] = g_LEDS[j].fadeToBlackBy(fadeAmt);  

    delay(50);
}