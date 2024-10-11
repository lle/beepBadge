#ifndef DEFINITIONS

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

// -------------- GPIO CONFIG --------------
#define BTN_U A1
#define BTN_L A3
#define BTN_R A0
#define BTN_D A2
#define BTN_A A7
#define BTN_B A6
#define DEBUG_LED 13
#define LEFT_IN 2
#define LEFT_OUT 3
#define LEFT_SENSE 4
#define RIGHT_IN 5
#define RIGHT_OUT 6
#define RIGHT_SENSE 7

// -------------- NEOPIXEL COLORS --------------
#define NEOPIX_PIN 9
#define NUMPIX 16

// -------------- NOTES TONES --------------
#define BUZZER_PIN 8    // change this to whichever pin you want to use
#define MIN_TONE 0
#define MAX_TONE 16

// Timestamps Schedulers
#define CURSOR_BLK_DELAY 50

#endif