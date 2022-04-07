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
.equ    INPUT_END_TIME, 2000000       @ default alarm timeout
.equ    INITIAL_STATE, 0              @ inital alarm hasnt been run




@ Entry point to the ASM portion of the program
main_asm:
    bl       init_btns
    bl       install_alrm_isr            @ install alarm
    bl       install gpio_isr           @ install gpio
    ldr      r0, =msg0                   @ store welcome message
    bl       printf            

loop:
    wfi         @ wait for interrupt
    b   loop    @ infinite loop

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


//author @emukperv

set_alarm:
    ldr     r2, =TIMER_BASE     @ get timer base reg address
    movs    r1, #1              @ set alarm
    str     r1, [r2, #TIMER_INTE_OFFSET]       @ enable timer interrupy
    ldr     r1, [r2, #TIMER_TIMELR_OFFSET]      @ get low time registers
    ldr     r3, =ltimer             @ current flash interval address
    ldr     r0, [r3]        @ get value of current flash interval
    add     r1, r0          @ add delay
    str     r1, [r2, #TIMER_ALARM0_OFFSET]      @ set new alarm with delay
    bx      lr      @ exit subtountine

install_alrm_isr:
    ldr     r2, =(PPB_BASE + M0PLUS_VTOR_OFFSET)        @ Get the address of the RAM vector table using the (PPB_BASE + M0PLUS_VTOR_OFFSET) register
    ldr     r1, [r2]                                    @ load address of RAM vector table
    movs    r2, #ALRM_ISR_OFFSET                        @ Store the address of the alrm_isr handler to the correct offset for ALARM0 in the vector table
    add     r2, r1                                      @ add offset to address
    ldr     r0, =alrm_isr                               @load address of alarm interrupt service handler
    str     r0, [r2]                                    @ store address of alrm_isr to correct offset in RAM vector table
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)   @ Disable the ALARM0 IRQ by writing the correct value to (PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)
    movs    r1, #1                                      
    str     r1 ,[r2]                                   
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)   @ Enable the ALARM0 IRQ by writing the correct value to (PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)
    movs    r1, #1          
    str     r1 ,[r2]                                    
    bx      lr                                          @ Exit subroutine

.thumb_func

alrm_isr:
push    {lr}        @ store link register to stack
ldr     r2, =TIMER_BASE     @ get timer_base address
movs    r1, #1          @ constant value to set alarm
str     r1, [r2, #TIMER_INTR_OFFSET]    @ reset alarm
ldr     r2, =alrmtimer      @ get address
movs    r0, #1          @ constant value to store in alrmtimer
str     r0, [r2]        @ store 1 in alrmtimer
pop     {pc}

//author @cduggan
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

//author @emukperv
gpio_isr:
    push    {lr}
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_PROC0_INTS2_OFFSET)      @ Read the interrupt status event from the (IO_BANK0_BASE + IO_BANK0_PROC0_INTS2_OFFSET) register
    ldr     r1, [r2]      @ acknowledge interrupt
    ldr     r0, =GPIO_BTN_DN_MSK        @ store mask in register
    cmp     r0, r1          @ compare interrupt with mask
    beq     falling_edge
    ldr     r0, =GPIO_BTN_UP_MSK    @mask for rising edge
    cmp     r0, r1
    beq     rising_edge
    bl      disable         @ disable alarm on falling edge
    ldr     r0, =GPIO_BTN_DN_MSK
    b       clear_interrupt
    pop     {pc}


    
.align 4
msg0:   .asciz "Use the GPIO 20 button to enter/input morse code.\n"
msg1:   .asciz "Use the GPIO 21/22 buttons to scroll back and forth."

@ Set data alignment
.data
    .align 4
    ltimer:         .word   INPUT_END_TIME  @ store current alarm time
    alrmtimer:         .word   INITIAL_STATE 
