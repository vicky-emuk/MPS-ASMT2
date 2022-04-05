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
    #ifndef PICO_DEFAULT_LED_PIN
    #warning blink example requires a board with a regular LED
    #else
//main program
    #endif
}