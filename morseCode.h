
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


struct MorseCode{
    int pointer;
    char * items;
};

struct morseCode * new_morse_code();
char morseCodeRead(struct MorseCode * input);
void morseCodeReset(struct MorseCode * input);
void morseCodeDot(struct MorseCode * input);
void morseCodeDash(struct MorseCode * input);
int morseCodeValid(struct MorseCode * input);

