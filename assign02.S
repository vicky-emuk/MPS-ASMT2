.syntax unified                                                 @ Specify unified assembly syntax
.cpu    cortex-m0plus                                           @ Specify CPU type is Cortex M0+
.thumb                                                          @ Specify thumb assembly for RP2040
.global main_asm                                                @ Provide program starting address to the linker
.align 4                                                        @ Specify code alignment


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
