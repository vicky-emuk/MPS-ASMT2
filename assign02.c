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
        const uint LED_PIN = PICO_DEFAULT_LED_PIN;
        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
        while(true){
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            gpio_put(LED_PIN, 0);
            sleep_ms(250);
        }
    #endif
}