#include "../inc/definitions.h"
#include "../inc/rgbControl.h"
#include "../inc/sequencerControl.h"
#include "../inc/soundControl.h"
#include "../inc/commControl.h"
#include "../inc/memoryControl.h"

#define 	_A	  1	  //	220   (technically A3)
#define 	_As	  2	  //	233
#define 	_B	  3	  //	247   (underscore before '_x' = 1 octave down)
#define 	C	    4 	//	262
#define 	Cs	  5	  //	277
#define 	D	    6	  //	294
#define 	Ds	  7	  //	311
#define 	E	    8	  //	330
#define 	F4	  9	  //	349  (calling this F4, because F is already used by Arduino)
#define 	Fs	  10	//	370
#define 	G	    11	//	392
#define 	Gs	  12	//	415
#define 	A	    13	//	440
#define 	As	  14	//	466
#define 	B	    15	//	494
#define 	C_	  16	//	523   (technically C5)  (underscore after 'x_' = next octave up)

const uint8_t SEQ_LGT = 16;
uint8_t sequencer[SEQ_LGT] =  {A,G,A,F4,     C,F4,_A,0,   A,G,A,F4,    C,F4,_A,0};  //16 timeslots available for sound sequencing
uint8_t sequencePreset[][SEQ_LGT] =  {  /*each number correspond to the tone requested for playback (see _TONES)*/                                         
                                          {A,G,A,F4,    C,F4,_A,0,    A,G,A,F4,      C,F4,_A,0    },  //popcorn part1
                                          {D,E,F4,E,    F4,D,E,D,     E,C,D,C,       _A,C,D,0     },  //popcorn part2
                                          {A,B,C_,B,    C_,A,B,A,     B,G,A,G,      A,_A,A,0      },  //popcorn part3
                                          {1,1,1,1,     1,1,1,1,      1,1,1,1,      1,1,1,1,      }  //twinkle twinkly little star
                                      };
uint32_t sequencerRGB[SEQ_LGT];
uint8_t cursorIndex = 0;

//PROTOTYPE
void sequencer_printIdWithLetter(uint8_t id);


// ------ GET/SET FUNCTIONS ------
uint8_t sequencer_getCursorIndex()
{
  return cursorIndex;
}

void sequencer_cursorIndexIncrement()
{
  cursorIndex++;
}

void sequencer_cursorIndexDecrement()
{
  cursorIndex--;
}

uint8_t sequencer_getValue(uint8_t index)
{
  return sequencer[index];
}

void sequencer_setValue(uint8_t index, uint8_t value)
{
  sequencer[index] = value;
}


// ------ FUNCTIONS -------
void sequencer_bootChkBtn()
{
  //scan for single-press on boot
  int8_t presetSelected = -1;  

  //Loading USER sequences from memory
  if(digitalRead(BTN_U) == LOW)
  {
    Serial.println("Load memory slot #1");
    if(memory_loadSlot1() == 0)
      presetSelected = 0;  //loading memory failed, load preset #0 instead
    else
      presetSelected = -1;  //loading successful, ignore preset
  }
  else if(digitalRead(BTN_D) == LOW)
  {
    Serial.println("Load memory slot #2");
    if(memory_loadSlot2() == 0)
      presetSelected = 1;  //loading memory failed, load preset #0 instead
    else
      presetSelected = -1;  //loading successful, ignore preset
  }

  //Loading PRE-PROGRAMMED sequences
  else if(digitalRead(BTN_R) == LOW)
    presetSelected = 2;
  else if(digitalRead(BTN_L) == LOW)
    presetSelected = 3;

  //Play animation
  if(presetSelected == -1)
  {
    rgbLed_loadAnimation();
    Serial.println("Sequence");
  }
  else
  {
    rgbLed_loadAnimation();
    Serial.print("preset selected=");
    Serial.println(presetSelected);
    for(int i=0; i<SEQ_LGT; i++)
    {
      sequencer[i] = sequencePreset[presetSelected][i];
    }
  }
}

void sequencer_print()
{
  for(int i=0; i<SEQ_LGT; i++)
  {
    sequencer_printIdWithLetter(sequencer[i]);
    
    if(i == cursorIndex)
      Serial.print("<");
    Serial.print("\t");
  }
  Serial.println();
}

void sequencer_play()
{
  Serial.println("PLAYING");

  for (int i=0; i<SEQ_LGT; i++)   // Remember, the array is twice the number of notes (notes + durations)
  {
    if(analogRead(BTN_B) == 0) //FORCE EXIT is B is being held
    {
      sound_errorBuzz();
      rgbLed_render();
      Serial.println("Force Exit");
      return;
    }

    rgbLed_highlight(i);
    Serial.print("^");
    soundControl_playsound(sequencer[i]);
    Serial.print("\t");
  }

  if(comPort_isRightBadgeDetected() == true)  //if right badge detected, than send play signal to it 
  {
    comPort_setRightOutput();
    delay(80); //hold signal to ensure the next badge reads it
  }
  rgbLed_render(); //re-render without the rgbLed_highlight
  noTone(BUZZER_PIN);  // stop the waveform generation before the next note.
  Serial.println();
  sequencer_print(); //reprint sequencer to serial
  comPort_unsetRightOutput(); //de-assert playback gpio for the right badge 
}

void sequencer_printIdWithLetter(uint8_t id)
{
  switch (id) 
  {
    case 0:
      Serial.print("-");
      break;
    case 1:
      Serial.print("A3");
      break;
    case 2:
      Serial.print("A3s");
      break;
    case 3:
      Serial.print("B3");
      break;
    case 4:
      Serial.print("C4");
      break;
    case 5:
      Serial.print("C4s");
      break;
    case 6:
      Serial.print("D4s");
      break;
    case 7:
      Serial.print("D4s");
      break;
    case 8:
      Serial.print("E4");
      break;
    case 9:
      Serial.print("F4");
      break;
    case 10:
      Serial.print("F4s");
      break;
    case 11:
      Serial.print("G4");
      break;
    case 12:
      Serial.print("G4s");
      break;
    case 13:
      Serial.print("A4");
      break;
    case 14:
      Serial.print("A4s");
      break;
    case 15:
      Serial.print("B4");
      break;
    case 16:
      Serial.print("C5_");
      break;
  }
}