#include "definitions.h"

// ------ GET/SET ------
uint8_t soundControl_getTempo(void);

// ------ PLAYSOUND ------
void soundControl_init(void);
void soundControl_playsound(int8_t requestTone);

// ------ BUZZER ------
void sound_lowBuzz(void);
void sound_errorBuzz(void);
void sound_playPreview(uint16_t toneReq);

// ------ TEMPO CONTROL ------
void tempo_decrease(void);
void tempo_increase(void);
void tempo_beep(uint8_t tempoInput);