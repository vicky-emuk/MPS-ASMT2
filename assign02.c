#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "morseCode.h";

#define IS_RGBW true        // Will use RGBW format
#define NUM_PIXELS 1        // There is 1 WS2812 device in the chain
#define GPIO_PIN 28         // The GPIO pin that the light is connected to 

/**
 * @brief Wrapper function used to call the underlying PIO
 *        function that pushes the 32-bit RGB colour value
 *        out to the LED serially using the PIO0 block. The
 *        function does not return until all of the data has
 *        been written out.
 * 
 * @param pixel_grb The 32-bit colour value generated by urgb_u32()
 */
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}


/**
 * @brief Function to generate an unsigned 32-bit composit GRB
 *        value by combining the individual 8-bit paramaters for
 *        red, green and blue together in the right order.
 * 
 * @param r     The 8-bit intensity value for the red component
 * @param g     The 8-bit intensity value for the green component
 * @param b     The 8-bit intensity value for the blue component
 * @return uint32_t Returns the resulting composit 32-bit RGB value
 */
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return  ((uint32_t) (r) << 8)  |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}


/**
 * @brief Function to generate an unsigned 32-bit composit GRB
 *        value by combining the individual 8-bit paramaters for
 *        red, green and blue together in the right order.
 * 
 * @param r     The 8-bit intensity value for the red component
 * @param g     The 8-bit intensity value for the green component
 * @param b     The 8-bit intensity value for the blue component
 * @return uint32_t Returns the resulting composit 32-bit RGB value
 */
static inline void colour_change(int colour) {
    if(colour==4)  put_pixel(urgb_u32(0x00, 0x00, 0x6F));
    else if(colour==3)  put_pixel(urgb_u32(0x00, 0x6F, 0x00));
    else if(colour==2)  put_pixel(urgb_u32(0x6F, 0x40, 0x00));
    else if(colour==1)  put_pixel(urgb_u32(0x6F, 0x00, 0x00));
    else if(colour==0)  put_pixel(urgb_u32(0x00, 0x00, 0x00));
}
int main() {
    stdio_init_all();
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_FALL, true); // init GPIO 21 for falling edge detection
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_RISE, true);

    printf("Welcome to group 23's Morse Code Game!\nHow to play?:\n To play, you simply have to enter the correct morse code sequence for the word (or character) displayed!\n Please select the difficulty you would like to play on by entering the corresponding morse code character\n");
    printf("Level 1 (EASY) : - - - - -");
    printf("Level 1 (HARD) : . - - - -");
    printf("Level 2 (EASY) : . . - - -");
    printf("Level 2 (HARD) : . . . - -");
    main_asm();
 
    // Initialise the PIO interface with the WS2812 code
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, GPIO_PIN, 800000, IS_RGBW);

    // Set the color to red at half intensity
    colour_change(4);
    sleep_ms(500);

    FILE *dictionary;
    dictionary = (fopen("dictionary.txt", "r"));
    if(fptr == NULL)
    {
        printf("File not present");
        exit(1);
    }

    // Set the color to green at half intensity
    colour_change(3);
    sleep_ms(500);

    // Set the color to blue at half intensity
    colour_change(2);
    sleep_ms(500);

    // Set the color to blue at half intensity
    colour_change(1);
    sleep_ms(500);

    // Set the color to blue at half intensity
    colour_change(0);
    sleep_ms(500);

    // Should never get here due to infinite while-loop.
    return 0;
}

int level_1_easy(){
    printf("Welcome to level 1! Easy edition...");
}
int level_1_hard(){
    printf("Welcome to level 1! Hard edition...");
}
int level_2_easy(){
    printf("Welcome to level 2! Easy edition...");
}
int level_2_hard(){
    printf("Welcome to level 2! Hard edition...");
}

void main_asm();
// Initialise a GPIO pin
void asm_gpio_init(uint pin) {
    gpio_init(pin);
}

// Set direction of a GPIO pin
void asm_gpio_set_dir(uint pin, bool out) {
    gpio_set_dir(pin, out);
}

// Get the value of a GPIO pin
bool asm_gpio_get(uint pin) {
    return gpio_get(pin);
}

// Set the value of a GPIO pin 
void asm_gpio_put(uint pin, bool value) {
    gpio_put(pin, value);
}

// Enable falling-edge interrupt 
void asm_gpio_set_irq_fall(uint pin) {
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL, true);
}

// Enable rising-edge interrupt
void asm_gpio_set_irq_rise(uint pin) {
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_RISE, true);
}

//All functions related to the morse code will go here.

//This struct is used for levels 1 and 2 and only contains the informations of a single char
struct morse_code_letter * new_morse_code_letter(char letter){
    
    struct morse_code_letter * result;
    result = malloc(sizeof(struct morse_code_letter));
    result -> lives = 3;
    result-> letter = letter;
    int charPosition = (letter < 97 ? letter - 64 : letter - 96);
    result -> morseCode = malloc(sizeof(char)*6);
    strcpy(getMorseCode(charPosition), result->morseCode);
    result->pointer = 0;
    return result;
}


//This struct is used for levels 3 and 4 and contains a full word as well as 
//the morsecode representation of that word in Morse Code separated by spaces
struct morse_code_word * new_morse_code_word(char * word){
    
    struct morse_code_word * result;
    result = malloc(sizeof(struct morse_code_word));
    result -> lives = 3;
    result-> word = malloc(sizeof(char)*6);
    strcpy(result->word,word );
    int length = strlen(result->word);
    result->morseCode = malloc(sizeof(char));
    for(int i = 0; i< length; i++){
        int charPosition = (result->word[i] < 97 ? result->word[i] - 64 : result->word[i] - 96);
        char * morseCodeForLetter = getMorseCode(charPosition);
        char * temp = malloc(sizeof(char) * (strlen(result->morseCode)+ 
        strlen(morseCodeForLetter)+1));
        strcpy(temp,result->morseCode);
        strcpy(temp, morseCodeForLetter);
        if(i<length-1){
            char * space = " ";
            strcpy(temp, space);
            
        }
        free(result->morseCode);
        result->morseCode = temp;
        free (morseCodeForLetter);
    }
    result->pointer = 0;
    return result;
}

char * getMorseCode( int letterPos){
    char morsecode[26][5] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","..--"};
    char * morse = malloc(sizeof(char) * strlen(morsecode[letterPos-1]));
    strcpy(morse, morsecode[letterPos-1]);
    return morse;
}