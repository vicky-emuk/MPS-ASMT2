
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


struct morse_code{
    int pointer;
    char * items;
};

struct morse_code * new_morse_code();
char morseCodeRead(struct morse_code * input);
void morseCodeReset(struct morse_code * input);
void morseCodeDot(struct morse_code * input);
void morseCodeDash(struct morse_code * input);
int morseCodeValid(struct morse_code * input);

