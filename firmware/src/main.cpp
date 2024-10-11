#include "../inc/definitions.h"
#include "../inc/testModule.h"
#include "../inc/rgbControl.h"
#include "../inc/buttonControl.h"
#include "../inc/sequencerControl.h"
#include "../inc/soundControl.h"
#include "../inc/commControl.h"
#define F_CPU 8000000L    //<---------- hardware requirement, assume Vcc worse case 2.7V, 8Mhz ext.crystal recommended (see datasheet Figure 29-1)

// ****************************************************
// --------- INITIALIZATION --------- 
// ****************************************************
void setup()
{
  button_init();
  comPort_init();
  soundControl_init();
  rgbLed_init();

  pinMode(DEBUG_LED, OUTPUT);
  digitalWrite(DEBUG_LED, HIGH);
  Serial.begin(115200);
  Serial.println("\n\n---------\n\rBEEP BADGE\n\rby Lambert\n\rOct.2024\n\r---------");
  sequencer_bootChkBtn(); //load different preset depending on which d-pad button is pressed

  if(analogRead(BTN_A) == 0 && analogRead(BTN_B) == 0) //if A and B are held down during boot, enter neopixel test mode
    rgbLed_testLoopLocking();

  rgbLed_render();
  sequencer_print();
  digitalWrite(DEBUG_LED, LOW);
}


// ****************************************************
// --------- MAIN LOOP --------- 
// ****************************************************
uint32_t ts_blink = 0;
uint32_t ts_btnScan = 0;
#define BTN_SCAN_DEFAULT 135
#define BTN_SCAN_FAST 90
uint16_t BTN_SCAN_DELAY = BTN_SCAN_DEFAULT;
void loop() 
{
  comPort_checkLeftPlaySignal();

  if(millis() - ts_btnScan > BTN_SCAN_DELAY)
  {
    ts_btnScan = millis();
    button_listener();

    if(button_isAnyHold() == true) //if the user is holding the button, accelerate btn_scan
      BTN_SCAN_DELAY = BTN_SCAN_FAST;
    else
      BTN_SCAN_DELAY = BTN_SCAN_DEFAULT;
  }

  rgbLed_autoReactivateBlinker();
  if(millis() - ts_blink > CURSOR_BLK_DELAY && rgbLed_getIsBlinkerEnabled() == 1) //
  {
    ts_blink = millis();
    rgbLed_cursorBlinkRun();
  }
}