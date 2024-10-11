#include "../inc/definitions.h"
#include "../inc/rgbControl.h"
#include "../inc/sequencerControl.h"
#include "../inc/soundControl.h"
#include "../inc/buttonControl.h"

Adafruit_NeoPixel neopix(NUMPIX, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
const uint32_t _COLORS[] = {0x222222, /*first note is silent note*/
                            0xff0000,0xff3300,0xcc6600,0xff9900,0xffff00,0xccff33,0xffff66,0xccff66,
                            0x99ff66,0x66ff66,0x00ff33,0x66ffcc,0x00ffff,0x33ccff,0x3399ff,0x0000cc};
uint32_t blinkerMask = 0xFFFFFF;                            
#define BLINKSTOP_HOLDTIME 1500  //how long the cursor should stop blinking when the user is changing the pitch
uint8_t isBlinkerEnabled = 1;
uint32_t ts_blinkerStop = 0;  //timestamp recording when the cursor blinker stopped
uint8_t isTempoVisible = 0;

void rgbLed_init()
{
  neopix.begin();
  neopix.setBrightness(20);
}

void rgbLed_test()
{
  neopix.fill(0x550000, 0, 16);
  neopix.show();
  delay(100);
  neopix.fill(0x005500, 0, 16);
  neopix.show();
  delay(100);
  neopix.fill(0x000055, 0, 16);
  neopix.show();
  delay(100);
}

void rgbLed_saveAnimation()
{
  neopix.clear();
  for(uint8_t i=0; i<neopix.numPixels(); i++) 
  {
    neopix.setPixelColor(i,0x00FF00);         //  Set pixel's color (in RAM)
    neopix.show();                          //  Update strip to match
    delay(25);                           //  Pause for a moment
  }
  delay(50);
}

void rgbLed_loadAnimation()
{
  neopix.clear();
  for(uint8_t i=0; i<neopix.numPixels(); i++) 
  {
    neopix.setPixelColor(15-i,0x00FF00);         //  Set pixel's color (in RAM)
    neopix.show();                          //  Update strip to match
    delay(25);                           //  Pause for a moment
  }
  delay(50);
}

uint8_t rgbLed_getIsBlinkerEnabled()
{
  return isBlinkerEnabled;
}

void rgbLed_blinkMask_disable()
{
  isBlinkerEnabled = 0;
  blinkerMask = 0xFFFFFF;
  ts_blinkerStop = millis();
}

void rgbLed_blinkMask_enable()
{
  isBlinkerEnabled = 1;
}

void rgbLed_tempoBar_activate()
{
  isBlinkerEnabled = 0;
  isTempoVisible = 1;
  blinkerMask = 0xFFFFFF;
  ts_blinkerStop = millis();
}

void rgbLed_tempoBar_disable()
{
  isTempoVisible = 0;
}

void rgbLed_autoReactivateBlinker()
{
  if(millis() - ts_blinkerStop > BLINKSTOP_HOLDTIME) //verify if it is time to re-enable cursor blinking
  {
    if(isTempoVisible == 1) //reactivate the sequencer once the user is done with the tempo selector 
    {
      rgbLed_render();
      rgbLed_tempoBar_disable();
    }
    else if(isBlinkerEnabled == 0)
    {
      rgbLed_render();
      rgbLed_blinkMask_enable();
    }
  }
}

void rgbLed_render()
{
  for(int i=0; i<NUMPIX; i++) 
  {
    int8_t requestedTone = sequencer_getValue(i);
    uint32_t targetColor = _COLORS[requestedTone];
    neopix.setPixelColor(i, targetColor);
  }
  neopix.show();   // Send the updated pixel colors to the hardware.
}

void rgbLed_highlight(uint8_t requestedIndex)
{
  for(int i=0; i<NUMPIX; i++) 
  {
    int8_t requestedTone = sequencer_getValue(i);
    uint32_t targetColor = _COLORS[requestedTone];
    if(i == requestedIndex)
    {
      neopix.setPixelColor(i, 0xFFFFFF);
    }
    else
    {
      neopix.setPixelColor(i, targetColor);
    }
  }
  neopix.show();   // Send the updated pixel colors to the hardware.
}

void rgbLed_cursorBlinkRun()
{
  //make it blink
  if(blinkerMask != 0x000000 && isBlinkerEnabled == 1) 
      blinkerMask = 0x000000;
  else
    blinkerMask = 0xFFFFFF;

  //load the Tone at the cursor index
  int8_t requestedTone = sequencer_getValue(sequencer_getCursorIndex()); 

  //load the matching color for that Tone
  uint32_t targetColor = _COLORS[requestedTone];
  uint32_t outputColor = targetColor & blinkerMask;
  neopix.setPixelColor(sequencer_getCursorIndex(), outputColor);
  neopix.show();
}

void rgbLed_renderTempo(uint16_t requestedTempo)
{
  neopix.clear();
  uint8_t count = map(requestedTempo, 60,210,1,16);
  neopix.fill(0xFF0000, 0, count);
  neopix.show();
}


// ---------------------------- testing function --------------------
void colorWipe(uint32_t color, int wait) {
  for(uint8_t i=0; i<neopix.numPixels(); i++) { // For each pixel in neopix...
    neopix.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    neopix.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      neopix.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(uint8_t c=b; c<neopix.numPixels(); c += 3) {
        neopix.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      neopix.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // neopix.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    neopix.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // neopix.rainbow(firstPixelHue, 1, 255, 255, true);
    neopix.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      neopix.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(uint8_t c=b; c<neopix.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (neopix.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / neopix.numPixels();
        uint32_t color = neopix.gamma32(neopix.ColorHSV(hue)); // hue -> RGB
        neopix.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      neopix.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void rgbLed_testLoopLocking()
{
  while(1)
  {
    // Fill along the length of the strip in various colors...
    colorWipe(neopix.Color(255,   0,   0), 50); // Red
    colorWipe(neopix.Color(  0, 255,   0), 50); // Green
    colorWipe(neopix.Color(  0,   0, 255), 50); // Blue

    // Do a theater marquee effect in various colors...
    theaterChase(neopix.Color(127, 127, 127), 50); // White, half brightness
    theaterChase(neopix.Color(127,   0,   0), 50); // Red, half brightness
    theaterChase(neopix.Color(  0,   0, 127), 50); // Blue, half brightness

    rainbow(10);             // Flowing rainbow cycle along the whole strip
    theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  }
}