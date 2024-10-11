#include "../inc/definitions.h"
#include "../inc/rgbControl.h"
#include "../inc/sequencerControl.h"
#include "../inc/soundControl.h"
#include "../inc/buttonControl.h"
#include "../inc/memoryControl.h"

#define HELD_DELAY 500

void button_init()
{
  pinMode(BTN_U, INPUT_PULLUP);
  pinMode(BTN_L, INPUT_PULLUP);
  pinMode(BTN_R, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_A, INPUT);
  pinMode(BTN_B, INPUT);
}

bool button_isAnyPressed()
{
  bool isPressed = false;
  if(analogRead(BTN_A) == 0 || analogRead(BTN_B) == 0 )
    isPressed = true;
  else if(digitalRead(BTN_U) == LOW || digitalRead(BTN_D) == LOW || digitalRead(BTN_L) == LOW || digitalRead(BTN_R) == LOW)
    isPressed = true;
  
  return isPressed;
}

bool button_isAnyHold()
{
  static uint32_t ts_lastBtnPressed = 0;
  static bool pressDetected = false;
  if(button_isAnyPressed() == true && pressDetected == false) //if a button was not pressed during the last scan AND is pressed now
  {
    ts_lastBtnPressed = millis(); //record time of btn press detected
    pressDetected = true;         //assert the flag
    return false;                 //return false for now, wait for next cycle to confirm if a button is held
  }
  else if(  button_isAnyPressed() == true 
            && pressDetected == true 
            && millis()-ts_lastBtnPressed>HELD_DELAY  ) //if the button is still being held after the 2nd cycle, then return true
  {
    return true;
  }
  else if( button_isAnyPressed() == false && pressDetected == true) //if the user stop holding the button, return false
  {
    pressDetected = false;
    return false;
  }
  return false;
}

void button_listener()
{
  //  ------ PLAYBACK and TEMPO CONTROL ------
  if( analogRead(BTN_A) == 0 ) 
  {
    sequencer_play();
  }
  else if( analogRead(BTN_B) == 0 && digitalRead(BTN_R) == LOW) //increase tempo
  {
    rgbLed_blinkMask_disable();
    tempo_increase();
  }
  else if( analogRead(BTN_B) == 0 && digitalRead(BTN_L) == LOW) //decrease tempo
  {
    rgbLed_blinkMask_disable();
    tempo_decrease();
  }
  else if( analogRead(BTN_B) == 0 && digitalRead(BTN_U) == LOW) //save animation slot1
  {
    memory_saveSlot1();
    rgbLed_saveAnimation();
    rgbLed_render();
  }
  else if( analogRead(BTN_B) == 0 && digitalRead(BTN_D) == LOW) //save animation slot2
  {
    memory_saveSlot2();
    rgbLed_saveAnimation();
    rgbLed_render();
  }
  else if( analogRead(BTN_B) == 0) //decrease tempo
  {
    rgbLed_tempoBar_activate();
    rgbLed_renderTempo(soundControl_getTempo());
  }


  // ------ INCREASE OR DECREASE PITCH ------
  else if( digitalRead(BTN_U) == LOW && analogRead(BTN_B) != 0) 
  { 
    uint8_t curr_index = sequencer_getCursorIndex();
    if(sequencer_getValue(curr_index) < MAX_TONE)
    {
      rgbLed_blinkMask_disable();
      sequencer_setValue(curr_index, sequencer_getValue(curr_index) + 1);
      sound_playPreview(sequencer_getValue(curr_index));
      rgbLed_render();
    }
    sequencer_print();
  }

  else if( digitalRead(BTN_D) == LOW && analogRead(BTN_B) != 0) 
  {  
    uint8_t curr_index = sequencer_getCursorIndex();
    if(sequencer_getValue(curr_index) > MIN_TONE)
    {
      rgbLed_blinkMask_disable();
      sequencer_setValue(curr_index, sequencer_getValue(curr_index) - 1);
      sound_playPreview(sequencer_getValue(curr_index));
      rgbLed_render();
    }
    sequencer_print();
  }


  // ------ MOVE LEFT OR RIGHT ------
  else if( digitalRead(BTN_L) == LOW ) 
  { 
    rgbLed_blinkMask_enable();
    if(sequencer_getCursorIndex() > 0) 
    {
      sequencer_cursorIndexDecrement();
      sound_lowBuzz();
      rgbLed_render();
    }
    else
    {
      sound_errorBuzz();
    }
    sequencer_print();
  }

  else if( digitalRead(BTN_R) == LOW ) 
  {  
    rgbLed_blinkMask_enable();
    if(sequencer_getCursorIndex() < 15) 
    {
      sequencer_cursorIndexIncrement();
      sound_lowBuzz();
      rgbLed_render();
    }
    else
    {
      sound_errorBuzz();
    }
    sequencer_print();
  }
}