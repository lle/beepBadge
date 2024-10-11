#include "definitions.h"

void comPort_init(void);

//LEFT BADGE
void comPort_checkLeftPlaySignal(void);

//RIGHT BADGE 
void comPort_setRightOutput(void);
void comPort_unsetRightOutput(void);
bool comPort_isRightBadgeDetected(void);