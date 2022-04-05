.syntax unified                                                 @ Specify unified assembly syntax
.cpu    cortex-m0plus                                           @ Specify CPU type is Cortex M0+
.thumb                                                          @ Specify thumb assembly for RP2040
.global main_asm                                                @ Provide program starting address to the linker
.align 4                                                        @ Specify code alignment



.equ    GPIO_BTN_DN_MSK, 0x00040000   @ Bit-18 for falling-edge event on GP20
.equ    GPIO_BTN_EN_MSK, 0x00400000   @ Bit-22 for falling-edge event on GP21
.equ    GPIO_BTN_UP_MSK, 0x04000000   @ Bit-26 for falling-edge event on GP22

.equ    GPIO_BTN_DN,  20              @ Specify pin for the "down" button
.equ    GPIO_BTN_EN,  21              @ Specify pin for the "enter" button
.equ    GPIO_BTN_UP,  22              @ Specify pin for the "up" button
.equ    GPIO_LED_PIN, 25              @ Specify pin for the built-in LED
.equ    GPIO_DIR_IN,   0              @ Specify input direction for a GPIO pin
.equ    GPIO_DIR_OUT,  1              @ Specify output direction for a GPIO pin

.equ    GPIO_ISR_OFFSET, 0x74         @ GPIO is int #13 (vector table entry 29)
.equ    ALRM_ISR_OFFSET, 0x40         @ ALARM0 is int #0 (vector table entry 16)



@ Entry point to the ASM portion of the program
main_asm:
    b       init_btns
    b       main_asm                                           @ Infinite loop



init_btns:
    push    {lr}                        @ Store the link register to the stack as we will call nested subroutines
    MOVS    R0, #GPIO_BTN_DN            
    BL      asm_gpio_init               @ Call subroutine to initialise the GPIO pin specified by R0
    MOVS    R0, #GPIO_BTN_DN            @ LED pin on PI PICO board
    BL      asm_gpio_set_irq            @ Call the subroutine (catch falling edge)

    MOVS    R0, #GPIO_BTN_EN            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_init               @ Call subroutine to init GPIO pin specified by R0
    MOVS    R0, #GPIO_BTN_EN            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_set_irq            

    MOVS    R0, #GPIO_BTN_UP            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_init               @ Call the subroutine to init GPIO pin specified by R0
    MOVS    R0, #GPIO_BTN_UP            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_set_irq

    pop     {pc}                        @ Pop the link register from the stack to the program counter


install_gpio_isr:
    LDR R2, =PPB_BASE                           
    LDR R1, =M0PLUS_VTOR_OFFSET
    ADD R2, R1
    LDR R1, [R2]
    MOVS R2, #GPIO_ISR_OFFSET
    ADD R2, R1
    LDR R0, =gpio_isr
    STR R0, [R2]
    LDR R0, =8192
    LDR R2, =PPB_BASE
    LDR R1, =M0PLUS_NVIC_ICPR_OFFSET
    ADD R1, R2
    STR R0, [R1]
    LDR R1, =M0PLUS_NVIC_ISER_OFFSET
    ADD R1, R2
    STR R0, [R1]
    BX LR

@ Set data alignment
.data
    .align 4
