#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

void main_asm();

void asm_gpio_init(uint pin) {
    gpio_init(pin);
}
void asm_gpio_set_dir(uint pin, bool out) {
    gpio_set_dir(pin, out);
}
bool asm_gpio_get(uint pin) {
    return gpio_get(pin);
}


int main() {
    stdio_init_all();
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_FALL, true); // init GPIO 21 for falling edge detection
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_RISE, true);

    printf("Welcome to group 23's More Code Game!\nHow to play?:\n To play, you simply have to enter the correct morse code sequence for the word (or character) displayed!\n Please select the difficulty you would like to play on by entering the corresponding morse code character\n");
    main_asm();
    return(0);

}