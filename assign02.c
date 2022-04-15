#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include "ws2812.pio.h"

void main_asm(); 
char userInput[30];
int current_index = 0;
int level = 1;
int gameStage = 0;
absolute_time_t startTimer;
absolute_time_t lastInput;
int sinceLast;
int spaces;

void set_watchdog_update(){
    watchdog_update();
}


//called by the enter key to enter the current user's input and check it
void flush_input()
{
    if(gameStage==3)
    {
        gameStage=4;
        userInput[current_index]='*';
        current_index=0;
        printf("\n");
    }
}


//Tags on the proper character based on the user input
void add_input(int number) {
    if(gameStage==1)gameStage=2;
    else if (current_index<30&&gameStage==3) {
        if(level==1||level==2)
        {
            if (number == 0) {
                userInput[current_index] = '.';
                printf(".");
            } else if (number == 1) {
                userInput[current_index] = '-';
                printf("-");
            } else if (number == 2) {
                flush_input();
            } else if (number == 3){
                userInput[current_index] = '\0';
            }
        }
        else if(level==3||level==4)
        {
            if(sinceLast>1000000&&current_index!=0)
            {
                if (number == 0) {
                    userInput[current_index++] = ' ';
                    userInput[current_index] = '.';
                    printf(" .");
                } else if (number == 1) {
                    userInput[current_index++] = ' ';
                    userInput[current_index] = '-';
                    printf(" -");
                }
            }
            else
            {
                if (number == 0) {
                    userInput[current_index] = '.';
                    printf(".");
                } else if (number == 1) {
                    userInput[current_index] = '-';
                    printf("-");
                }
            }
        }
        current_index++;
    }
}


//This will decode morse code into the character it is meant to represent
char inputDecode(char* input)
{
    // Letters
    if(input[0]=='.'&&input[1]=='-'&&input[2]=='*')return 'A';
    else if(input[0]=='-'&&input[1]=='.'&&input[2]=='.'&&input[3]=='.'&&input[4]=='*')return 'B';
    else if(input[0]=='-'&&input[1]=='.'&&input[2]=='-'&&input[3]=='.'&&input[4]=='*')return 'C';
    else if(input[0]=='-'&&input[1]=='.'&&input[2]=='.'&&input[3]=='*')return 'D';
    else if(input[0]=='.'&&input[1]=='*')return 'E';
    else if(input[0]=='.'&&input[1]=='.'&&input[2]=='-'&&input[3]=='.'&&input[4]=='*')return 'F';
    else if(input[0]=='-'&&input[1]=='-'&&input[2]=='.'&&input[3]=='*')return 'G';
    else if(input[0]=='.'&&input[1]=='.'&&input[2]=='.'&&input[3]=='.'&&input[4]=='*')return 'H';
    else if(input[0]=='.'&&input[1]=='.'&&input[2]=='*')return 'I';
    else if(input[0]=='.'&&input[1]=='-'&&input[2]=='-'&&input[3]=='-'&&input[4]=='*')return 'J';
    else if(input[0]=='-'&&input[1]=='.'&&input[2]=='-'&&input[3]=='*')return 'K';
    else if(input[0]=='.'&&input[1]=='-'&&input[2]=='.'&&input[3]=='.'&&input[4]=='*')return 'L';
    else if(input[0]=='-'&&input[1]=='-'&&input[2]=='*')return 'M';
    else if(input[0]=='-'&&input[1]=='.'&&input[2]=='*')return 'N';
    else if(input[0]=='-'&&input[1]=='-'&&input[2]=='-'&&input[3]=='*')return 'O';
    else if(input[0]=='.'&&input[1]=='-'&&input[2]=='-'&&input[3]=='.'&&input[4]=='*')return 'P';
    else if(input[0]=='-'&&input[1]=='-'&&input[2]=='.'&&input[3]=='-'&&input[4]=='*')return 'Q';
    else if(input[0]=='.'&&input[1]=='-'&&input[2]=='.'&&input[3] == '*')return 'R';
    else if(input[0]=='.'&&input[1]=='.'&&input[2]=='.'&&input[3]=='*')return 'S';
    else if(input[0]=='-'&&input[1]=='*')return 'T';
    else if(input[0]=='.'&&input[1]=='.'&&input[2]=='-'&&input[3]=='*')return 'U';
     else if(input[0]=='.'&&input[1]=='.'&&input[2]=='.'&&input[3]=='-'&&input[4]=='*')return 'V';
     else if(input[0]=='.'&&input[1]=='-'&&input[2]=='-'&&input[3]=='*')return 'W';
     else if(input[0]=='-'&&input[1]=='.'&&input[2]=='.'&&input[3]=='-'&&input[4]=='*')return 'X';
     else if(input[0]=='-'&&input[1]=='.'&&input[2]=='-'&&input[3]=='-'&&input[4]=='*')return 'Y';
     else if(input[0]=='-'&&input[1]=='-'&&input[2]=='.'&&input[3]=='.'&&input[4]=='*')return 'Z';

     // Numbers
     else if(input[0]=='-'&&input[1]=='-'&&input[2]=='-'&&input[3]=='-'&&input[4]=='-'&&input[5]== '*')return '0';
     else if(input[0]=='.'&&input[1]=='-'&&input[2]=='-'&&input[3]=='-'&&input[4]=='-'&&input[5]== '*')return '1';
     else if(input[0]=='.'&&input[1]=='.'&&input[2]=='-'&&input[3]=='-'&&input[4]=='-'&&input[5]== '*')return '2';
     else if(input[0]=='.'&&input[1]=='.'&&input[2]=='.'&&input[3]=='-'&&input[4]=='-'&&input[5]== '*')return '3';
     else if(input[0]=='.'&&input[1]=='.'&&input[2]=='.'&&input[3]=='.'&&input[4]=='-'&&input[5]== '*')return '4';
     else if(input[0]=='.'&&input[1]=='.'&&input[2]=='.'&&input[3]=='.'&&input[4]=='.'&&input[5]== '*')return '5';
     else if(input[0]=='-'&&input[1]=='.'&&input[2]=='.'&&input[3]=='.'&&input[4]=='.'&&input[5]== '*')return '6';
     else if(input[0]=='-'&&input[1]=='-'&&input[2]=='.'&&input[3]=='.'&&input[4]=='.'&&input[5]== '*')return '7';
     else if(input[0]=='-'&&input[1]=='-'&&input[2]=='-'&&input[3]=='.'&&input[4]=='.'&&input[5]== '*')return '8';
     else if(input[0]=='-'&&input[1]=='-'&&input[2]=='-'&&input[3]=='-'&&input[4]=='.'&&input[5]== '*')return '9';
    else return '?';
}

//This is used for the levels to translate each charachter into morse code to print to the screen.
void printMorse(char chosenChar)
{
    switch(chosenChar)
    {
        case 'A':
            printf(".-");
            break;
        case 'B':
            printf("-...");
            break;
        case 'C':
            printf("-.-.");
            break;
        case 'D':
            printf("-..");
            break;
        case 'E':
            printf(".");
            break;
        case 'F':
            printf("..-.");
            break;
        case 'G':
            printf("--.");
            break;
        case 'H':
            printf("....");
            break;
        case 'I':
            printf("..");
            break;       
        case 'J':
            printf(".---");
            break;
        case 'K':
            printf("-.-");
            break;
        case 'L':
            printf(".-..");
            break;
        case 'M':
            printf("--");
            break;
        case 'N':
            printf("-.");
            break;
        case 'O':
            printf("---");
            break;
        case 'P':
            printf(".--.");
            break;
        case 'Q':
            printf("--.-");
            break;
        case 'R':
            printf(".-."); 
            break;    
        case 'S':
            printf("...");
            break;
        case 'T':
            printf("-");
            break;
        case 'U':
            printf("..-");
            break;       
        case 'V':
            printf("...-");
            break;
        case 'W':
            printf(".--");
            break;
        case 'X':
            printf("-..-");
            break;
        case 'Y':
            printf("-.--");
            break;
        case 'Z':
            printf("--..");
            break;
        case '0':
            printf("-----");
            break;
        case '1':
            printf(".----");
            break;
        case '2':
            printf("...--");
            break;
        case '3':
            printf("...--"); 
            break;    
        case '4':
            printf("....-");
            break;
        case '5':
            printf(".....");
            break;
        case '6':
            printf("-....");
            break;
        case '7':
            printf("--...");
            break;
        case '8':
            printf("---..");
            break;
        case '9':
            printf("----."); 
            break;  
        default :
            printf("Error");
    }
}

// This function starts a timer to tell if a user has waiter between inputs
void start_timer(){
    startTimer = get_absolute_time();
    sinceLast= (int) absolute_time_diff_us(lastInput, startTimer);
}

//This function ends the timer to check if the user has waited between inputs
//returns a the differenct between times to be parsed
int end_timer(){
    absolute_time_t end_time = get_absolute_time();
    lastInput = get_absolute_time();
    return (int) absolute_time_diff_us(startTimer, end_time); // find time inbetween, parse int 
}
void levels_increment(){
    if(gameStage==1)
    {
        if(level==4) level=1;
        else level++;
        printf("New level is %i\n",level);
    } 
}


#define IS_RGBW true        // Will use RGBW format
#define NUM_PIXELS 1        // There is 1 WS2812 device in the chain
#define GPIO_PIN 28         // The GPIO pin that the light is connected to 

 int levelOne();
 int levelTwo();
 int levelThree();
 int levelFour();

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
    if(colour==4)  put_pixel(urgb_u32(0x00, 0x00, 0x5F));
    else if(colour==3)  put_pixel(urgb_u32(0x00, 0x5F, 0x00));
    else if(colour==2)  put_pixel(urgb_u32(0x5F, 0x2F, 0x00));
    else if(colour==1)  put_pixel(urgb_u32(0x5F, 0x00, 0x00));
    else if(colour==0)  put_pixel(urgb_u32(0x00, 0x00, 0x00));
}

    printf("Welcome to group 23's Morse Code Game!\nHow to play?:\n To play, you simply have to enter the correct morse code sequence for the word (or character) displayed!\n Please select the difficulty you would like to play on by entering the corresponding morse code character\n");
    printf("Level 1 (EASY) : - - - - -");
    printf("Level 1 (HARD) : . - - - -");
    printf("Level 2 (EASY) : . . - - -");
    printf("Level 2 (HARD) : . . . - -");
    main_asm();
    watchdog_enable(9000, 1);
    if (watchdog_caused_reboot()){
        printf("Reboot Sucessful!\n");
    }
    else{
        printf("Not Watchdog Reboot.\n");
    }

    // Initialise all STDIO as we will be using the GPIOs
    stdio_init_all();
    srand(87);
 
    // Initialise the PIO interface with the WS2812 code
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, GPIO_PIN, 800000, IS_RGBW);

    printf("Welcome to group 23's Morse Code Game!\nHow to play?:\nTo play, you simply have to enter the correct morse code sequence for the word (or character) displayed!\nPlease select the difficulty you would like to play on by entering the corresponding morse code character\n");
    // Set the color to red at half intensity
    colour_change(4);


    gameStage=1;
    // This code waits for user input to select the lever using the level select button(gp20)
    printf("Current level is %i\n",level);
    while(gameStage==1)
    {
        
    }

    int complete=1;

    switch(level)
    {
        case 1:
            complete=levelOne();
            if(complete==0)break;
        case 2:
            complete=levelTwo();
            if(complete==0)break;
        case 3:
            complete=levelThree();
            if(complete==0)break;
        case 4:
            complete=levelFour();
            break;
        default :
        printf("Error");
        levelOne();
    }

    if(complete==1)printf("\ncongratulations");
    else printf("\nYou lose, good day sir!\n");

    return 0;

}

//In this level it waits for the enter button(gp22) to be pressed before parsing the user input
//it also prints out the morse code for each letter that the player has to enter
int levelOne()
{ 
    printf("\nBeginning with level one");
    int lives=3;
    int correctInARow=0;
    char characters[36]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
    while((correctInARow<5)&&(lives>0))
    {
        colour_change(lives);

        uint32_t random=to_ms_since_boot(get_absolute_time());
        random%=36;
        printf("\n%c\n",characters[random]);
        printMorse(characters[random]);
        printf("\n");

        gameStage=3;
        //waits here while the user inputs their answer. breaks once they hit the enter button.
        while(gameStage==3)
        {   
            int re=4;
            printf("",re);
        }

        char inputChar=inputDecode(userInput);
        printf("%c\n",inputChar);

        if(inputChar==characters[random])
        {
            printf("\ncorrect!\n");
            correctInARow++;
            if(lives<3)lives++;
        }
        else
        {
            printf("\nIncorrect!\n");
            correctInARow=0;
            lives--;
        }
    }   
    if(lives==0)return 0;
    return 1; 
}

// This level acts the same as level one but will not print out the 
//morse code of the letter the player has to enter
int levelTwo()
{ 
    printf("\nLevel Two");
    int lives=3;
    int correctInARow=0;
    char characters[36]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
    while((correctInARow<5)&&(lives>0))
    {
        colour_change(lives);

        uint32_t random=to_ms_since_boot(get_absolute_time());
        random%=36;
        printf("\n%c\n",characters[random]);

        gameStage=3;
        //waits here while the user inputs their answer. breaks once they hit the enter button.
        while(gameStage==3)
        {   
            int re=4;
            printf("",re);
        }

        char inputChar=inputDecode(userInput);
        printf("%c\n",inputChar);

        if(inputChar==characters[random])
        {
            printf("\ncorrect!\n");
            correctInARow++;
            if(lives<3)lives++;
        }
        else
        {
            printf("\nIncorrect!\n");
            correctInARow=0;
            lives--;
        }
    }   
    if(lives==0)return 0;
    return 1; 
}

//This level prints out one of eight words for the player to enter plus the 
//morse code for each input. If the player waits a while between inputs then
// a space will be added before the next input
int levelThree()
{ 
    printf("\nLevel Three");
    int lives=3;
    int correctInARow=0;
    char words[8][7]={"DAILY","MOUNT","FRESH","QUICK","ZEBRA","YOUNG","EXTRA","WEDGE"};
    while((correctInARow<5)&&(lives>0))
    {
        colour_change(lives);

        uint32_t random=to_ms_since_boot(get_absolute_time());
        random%=8;
        printf("\n%s\n",words[random]);
        for(int i=0;i<5;i++)
        {
            printMorse(words[random][i]);
            printf(" ");
        }
        printf("\n");

        gameStage=3;
        //waits here while the user inputs their answer. breaks once they hit the enter button.
        while(gameStage==3)
        {   
            int re=4;
            printf("",re);
        }
        //This code is all dedicated to decoding the user's input to compare it to the proper input
        char userWord[7];
        char currentChar;
        int i=0;
        int j=0;
        int k=0;
        char currentLetter[10];
        int decoded=0;
        while(decoded==0)
        {
            currentChar=userInput[i++];
            char inputChar;
            switch(currentChar)
            {
                case '*':
                    currentLetter[j]=currentChar;
                    inputChar=inputDecode(currentLetter);
                    userWord[k]=inputChar;
                    decoded=1;
                    break;
                case ' ':
                    currentLetter[j]='*';
                    inputChar=inputDecode(currentLetter);
                    userWord[k++]=inputChar;
                    j=0;
                    break;
                default:
                    currentLetter[j++]=currentChar;
                    //int re=4;
                    //printf("",re);
            }
        }
        //Compares the two inputs once the user input is decoded.
        if(strcmp(userWord, words[random])==0)
        {
            printf("correct!\n");
            correctInARow++;
            if(lives<3)lives++;
        }
        else
        {
            printf("Incorrect input!\n");
            correctInARow=0;
            lives--;
        }
    }    
    if(lives==0)return 0;
    return 1;
}


//This level acts much the same as level three but doesn't provide the input
// for each word
int levelFour()
{ 
    printf("\nLevel Four");
    int lives=3;
    int correctInARow=0;
    char words[8][7]={"DAILY","MOUNT","FRESH","QUICK","ZEBRA","YOUNG","EXTRA","WEDGE"};
    while((correctInARow<5)&&(lives>0))
    {
        colour_change(lives);

        uint32_t random=to_ms_since_boot(get_absolute_time());
        random%=8;
        printf("\n%s\n",words[random]);

        gameStage=3;
        //waits here while the user inputs their answer. breaks once they hit the enter button.
        while(gameStage==3)
        {   
            int re=4;
            printf("",re);
        }
        //This code is all dedicated to decoding the user's input to compare it to the proper input
        char userWord[7];
        char currentChar;
        int i=0;
        int j=0;
        int k=0;
        char currentLetter[10];
        int decoded=0;
        while(decoded==0)
        {
            currentChar=userInput[i++];
            char inputChar;
            switch(currentChar)
            {
                case '*':
                    currentLetter[j]=currentChar;
                    inputChar=inputDecode(currentLetter);
                    userWord[k]=inputChar;
                    decoded=1;
                    break;
                case ' ':
                    currentLetter[j]='*';
                    inputChar=inputDecode(currentLetter);
                    userWord[k++]=inputChar;
                    j=0;
                    break;
                default:
                    currentLetter[j++]=currentChar;
                    //int re=4;
                    //printf("",re);
            }
        }
        //Compares the two inputs once the user input is decoded.
        if(strcmp(userWord, words[random])==0)
        {
            printf("correct!\n");
            correctInARow++;
            if(lives<3)lives++;
        }
        else
        {
            printf("Incorrect input!\n");
            correctInARow=0;
            lives--;
        }
    }    
    if(lives==0)return 0;
    return 1;
}

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
