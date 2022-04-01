// Must declare the main assembly entry point before use.
void main_asm();

/*
 * Main entry point for the code - simply calls the main assembly function.
 */
int main() {
    stdio_init_all();
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_FALL, true); // init GPIO 21 for falling edge detection
    gpio_set_irq_enabled(21, GPIO_IRQ_EDGE_RISE, true);

    printf("Welcome to group 23's Morse Code Game!\nHow to play?:\n To play, you simply have to enter the correct morse code sequence for the word (or character) displayed!\n Please select the difficulty you would like to play on by entering the corresponding morse code character\n");
    main_asm();
    return(0);
}
