#include "definitions.h"

// ------ GET/SET ------
uint8_t sequencer_getCursorIndex(void);
void sequencer_cursorIndexIncrement(void);
void sequencer_cursorIndexDecrement(void);
uint8_t sequencer_getValue(uint8_t index);
void sequencer_setValue(uint8_t index, uint8_t value);

// ------ FUNCTIONS ------
void sequencer_bootChkBtn(void);
void sequencer_print(void);
void sequencer_play(void);