#include "../inc/commControl.h"
#include "../inc/sequencerControl.h"

void comPort_init()
{
    pinMode(LEFT_IN, INPUT_PULLUP);
    pinMode(LEFT_OUT, OUTPUT);
    pinMode(LEFT_SENSE, INPUT_PULLUP);
    pinMode(RIGHT_IN, INPUT_PULLUP);
    pinMode(RIGHT_OUT, OUTPUT);
    pinMode(RIGHT_SENSE, INPUT_PULLUP);

    digitalWrite(LEFT_OUT, HIGH);
    digitalWrite(RIGHT_OUT, HIGH);
}

//LEFT BADGE
void comPort_checkLeftPlaySignal()
{
    if(digitalRead(LEFT_IN) == LOW)
    {
        sequencer_play();
    }
}

//RIGHT BADGE
void comPort_setRightOutput()
{
    digitalWrite(RIGHT_OUT, LOW);
}

void comPort_unsetRightOutput()
{
    digitalWrite(RIGHT_OUT, HIGH);
}

bool comPort_isRightBadgeDetected(void)
{
    if(digitalRead(RIGHT_SENSE) == LOW)
        return true;    //badge detected
    else   
        return false;   //no badge
}