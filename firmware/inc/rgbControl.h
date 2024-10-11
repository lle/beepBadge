#include "definitions.h"

// ------ GET/SET ------
uint8_t rgbLed_getIsBlinkerEnabled(void);
void rgbLed_blinkMask_disable(void);
void rgbLed_blinkMask_enable(void);
void rgbLed_tempoBar_activate(void);
void rgbLed_tempoBar_disable(void);
void rgbLed_renderTempo(uint16_t requestedTempo);
void rgbLed_highlight(uint8_t requestedIndex);

// ------ Functions ------
void rgbLed_init(void);
void rgbLed_test(void);
void rgbLed_saveAnimation(void);
void rgbLed_loadAnimation(void);
void rgbLed_autoReactivateBlinker(void);
void rgbLed_render(void);
void rgbLed_cursorBlinkRun();

// ------ testing function ------
void rgbLed_testLoopLocking(void);