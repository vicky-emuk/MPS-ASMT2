#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


struct morse_code_letter{
    int lives;
    int pointer;
    char letter;
    char * morseCode;
    int sizeOfMorsecode;
};
struct morse_code_word
{
    int lives;
    int pointer;
    char * word;
    char * morseCode;
    int sizeOfMorsecode;
    
};


struct morse_code_letter * new_morse_code_letter(char letter);
struct morse_code_word * new_morse_code_word (char * word );
char morseCodeLetterRead(struct morse_code_letter * input);
void morseCodeLetterReset(struct morse_code_letter * input);
void morseCodeLetterDot(struct morse_code_letter * input);
void morseCodeLetterDash(struct morse_code_letter * input);
int morseCodeLetterValid(struct morse_code_letter * input);
int morseCodeInputLetter(int dotOrDash, struct morse_code_letter * input, int select );
void printUpToErrorLetter(struct morse_code_letter * input);

char * morseCodeWordRead(struct morse_code_word * input);
void morseCodeWordReset(struct morse_code_word * input);
void morseCodeWordDot(struct morse_code_word * input);
void morseCodeWordDash(struct morse_code_word * input);
int morseCodeWordValid(struct morse_code_word * input);
int morseCodeInputWord(int dotOrDash, struct morse_code_word * input, int select );
void printUpToErrorWord(struct morse_code_word * input);

char * getMorseCode();

void printDot();
void printDash();
