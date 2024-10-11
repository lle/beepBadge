#include "../inc/definitions.h"
#include "../inc/rgbControl.h"
#include "../inc/sequencerControl.h"
#include "../inc/soundControl.h"
#include "../inc/buttonControl.h"

#define TEMPO_MAX 210
#define TEMPO_MIN 60

// -------------- NOTES TONES --------------         
uint8_t tempo = 140;     // change this to make the song slower or faster         
int divider = 8;
int noteDuration = 0;
const uint16_t _TONES[] = {0,                                 //first note is 'silent'
                          220,233,247,262,  277,294,311,330,  //then start at tone A3, A3s...
                          349,370,392,415,  440,466,494,523}; //until ...C5


// ------ GET/SET ------
uint8_t soundControl_getTempo()
{
    return tempo;
}

// ------ FUNCTIONS ------
void soundControl_init()
{
  int wholenote = (60000 * 4) / soundControl_getTempo();    // this calculates the duration of a whole note in ms (60s/tempo)*4 beats
  noteDuration = (wholenote) / divider;
}

void soundControl_playsound(int8_t requestTone)
{
    int wholenote = (60000 * 4) / soundControl_getTempo();    // this calculates the duration of a whole note in ms (60s/tempo)*4 beats
    noteDuration = (wholenote) / divider;

    if(requestTone == 0) //  '0' means silent note
      noTone(BUZZER_PIN);  // stop the waveform generation before the next note.
    else
      tone(BUZZER_PIN, _TONES[requestTone], noteDuration*0.9); // we only play the note for 90% of the duration, leaving 10% as a pause

    // Wait for the specified duration before playing the next note.
    delay(noteDuration);  
}

// ------ BUZZER ------
void sound_lowBuzz()
{
  tone(BUZZER_PIN, 210, noteDuration*0.2);
  delay(noteDuration*0.2);
  noTone(BUZZER_PIN); 
}

void sound_errorBuzz()
{
  tone(BUZZER_PIN, 110, noteDuration*0.5);
  delay(noteDuration*0.5);
  noTone(BUZZER_PIN); 
}

void sound_playPreview(uint16_t toneReq)
{
 uint16_t previewDuration = noteDuration>>1; //play half a normal duration 
  tone(BUZZER_PIN, _TONES[toneReq], previewDuration*0.9);
  delay(previewDuration);
  noTone(BUZZER_PIN); 
}

// ------ TEMPO CONTROL ------
#define SLOW_TEMPO_BEEP 1175  //D6
#define MED_TEMPO_BEEP 1319      //E6
#define FAST_TEMPO_BEEP 1397      //F6
void tempo_decrease()
{
  if(tempo > TEMPO_MIN)
  {
    tempo = tempo - 10;
    rgbLed_renderTempo(tempo);
    tempo_beep(tempo);
    Serial.print("\ttempo=");
    Serial.println(tempo);
  }
}

void tempo_increase()
{
  if(tempo < TEMPO_MAX)
  {
    tempo = tempo + 10;
    rgbLed_renderTempo(tempo);
    tempo_beep(tempo);
    Serial.print("\ttempo=");
    Serial.println(tempo);
  }
}

void tempo_beep(uint8_t tempoInput)
{
  int temp_wholenote = (60000 * 4) / TEMPO_MAX;
  int temp_noteDuration = (temp_wholenote) / divider;
  tone(BUZZER_PIN, tempoInput*5, temp_noteDuration*0.9);
  delay(temp_noteDuration);
  noTone(BUZZER_PIN); 
}