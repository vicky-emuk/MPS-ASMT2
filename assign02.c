#include "morseCode.h"
// Must declare the main assembly entry point before use.
void main_asm();

/*
 * Main entry point for the code - simply calls the main assembly function.
 */
int main() {
    char morsecode[26][5] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","..--"};

    stdio_init_all();
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_FALL, true); // init GPIO 21 for falling edge detection
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_RISE, true);

    printf("Welcome to group 23's More Code Game!\nHow to play?:\n To play, you simply have to enter the correct morse code sequence for the word (or character) displayed!\n Please select the difficulty you would like to play on by entering the corresponding morse code character\n");
    main_asm();
    return(0);
}

//Creates and returns a new morse_code struct used to determine if the morse code entered is correct so far
struct morse_code * new_morse_code(){
    struct morse_code * result;
    result ->pointer = 0;
    char morseCodeArray[32] = {'-', 'e', 't', 'i','a','n','m','s','u','r','w','d','k','g'
    ,'o','h','v','f','?','l','?','p','j','b','x','c','y','z','q','?','?','\0'};
    result ->items = morseCodeArray; 
    return result;
}
char * getMorseFromChar(char input){
    
}
//read out which char the current morse code corresponds to
char morseCodeRead(struct morse_code * input){
    if(input->pointer > 31){
        return '?';
    }
    char output = input ->items[input->pointer];
    return output;
}

//reset the morse_code struct for next input
void morseCodeReset(struct morse_code * input){
    input->pointer = 0;
}
int morseCodeValid(struct morse_code * input){
    if (morseCodeRead(input) == '?' || morseCodeRead(input) == '-'){
        return 0;
    }
    return 1;
}

//inputs a new dot into the morse_code struct
void morseCodeDot(struct morse_code * input){

    input->pointer = ((input->pointer + 1) * 2)-1;
}


//inputs a new dash into the mors_code struct
void morseCodeDash(struct morse_code * input){

    input->pointer = ((input->pointer + 1) * 2);
}

