#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

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
