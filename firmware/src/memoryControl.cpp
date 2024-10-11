#include <EEPROM.h>
#include "../inc/definitions.h"
#include "../inc/memoryControl.h"
#include "../inc/sequencerControl.h"

#define sequence_length 16
void memory_saveSlot1(void)
{
    for(int i=0; i<sequence_length; i++)
    {
        int val = sequencer_getValue(i);
        EEPROM.write(i, val);
    }
    Serial.println("Sequence saved to memory slot #1");
}

#define memory_offsetSlot2 16
void memory_saveSlot2(void)
{
    for(int i=0; i<sequence_length; i++)
    {
        int val = sequencer_getValue(i);
        EEPROM.write(i+memory_offsetSlot2, val);
    }
    Serial.println("Sequence saved to memory slot #2");
}

uint8_t memory_loadSlot1(void)
{
    for(int i=0; i<sequence_length; i++)
    {
        int value = EEPROM.read(i);
        if(value == 255)
        {
            Serial.println("Memory slot is empty");
            return 0;
        }
        sequencer_setValue(i, value);
    }
    return 1;
}

uint8_t memory_loadSlot2(void)
{
    for(int i=0; i<sequence_length; i++)
    {
        int value = EEPROM.read(i+memory_offsetSlot2);
        if(value == 255)
        {
            Serial.println("Memory slot is empty");
            return 0;
        }
        sequencer_setValue(i, value);
    }
    return 1;
}