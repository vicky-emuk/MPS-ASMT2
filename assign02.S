#include "hardware/regs/addressmap.h"
#include "hardware/regs/io_bank0.h"
#include "hardware/regs/timer.h"
#include "hardware/regs/m0plus.h"

.syntax unified                                                 @ Specify unified assembly syntax
.cpu    cortex-m0plus                                           @ Specify CPU type is Cortex M0+
.thumb                                                          @ Specify thumb assembly for RP2040
.global main_asm                                                @ Provide program starting address to the linker
.align 4                                                        @ Specify code alignment



.equ    GPIO_BTN_DN_MSK, 0x00040000   @ Bit-18 for falling-edge event on GP20
.equ    GPIO_BTN_ENF_MSK, 0x00400000   @ Bit-22 for falling-edge event on GP21
.equ    GPIO_BTN_ENR_MSK, 0x00800000    @ Bit-22 for rising-edge event on GP21
.equ    GPIO_BTN_UP_MSK, 0x04000000    @ Bit-22 for falling-edge event on GP22

.equ    GPIO_BTN_DN,  20              @ Specify pin for the "down" button
.equ    GPIO_BTN_EN,  21              @ Specify pin for the "enter" button
.equ    GPIO_BTN_UP,  22              @ Specify pin for the "up" button
.equ    GPIO_LED_PIN, 25              @ Specify pin for the built-in LED
.equ    GPIO_DIR_IN,   0              @ Specify input direction for a GPIO pin
.equ    GPIO_DIR_OUT,  1              @ Specify output direction for a GPIO pin

.equ    GPIO_ISR_OFFSET, 0x74         @ GPIO is int #13 (vector table entry 29)
.equ    ALRM_ISR_OFFSET, 0x40         @ ALARM0 is int #0 (vector table entry 16)
.equ    INPUT_END_TIME, 3000000        @ Specify the default alarm timeout - 3 seconds
.equ    INITIAL_STATE, 0               @ init alarm has not been run




@ Entry point to the ASM portion of the program
main_asm:
    push    {lr}
    bl      init_btns           @ initalise button                                   @ Infinite loop
    bl      install_alarm_isr   @ install alarm
    bl      install_gpio_isr    @ instll gpio
    pop     {pc}
loop:
push    {lr}
    bl      set_alarm           @ loop to reset alarm for 3 seconds
    wfi                         @ wait for interrupt
    ldr     r2, =atimer         @ from labs - check if alar interrupt has ran
    ldr     r1, [r2]            @ load value
    movs    r0, #1              @ constant value for timer on
    cmp     r0, r1              @ compare with variable
    bne     loop                @ loop if not equal
    movs    r0, #0              @ reset alarm interrupt by storing 0
    str     r0, [r2]            @ storing in variable atimer
    movs    r0, #3              @ end of string
    movs    r1, #0              @ set next char to be inputted in next index
    bl      add_input           @ add user input
    pop     {pc}

level_select:
    


init_btns:
    push    {lr}                        @ Store the link register to the stack as we will call nested subroutines
    MOVS    R0, #GPIO_BTN_DN            
    BL      asm_gpio_init               @ Call subroutine to initialise the GPIO pin specified by R0
    MOVS    R0, #GPIO_BTN_DN            @ LED pin on PI PICO board
    BL      asm_gpio_set_irq_fall       @ Call the subroutine (catch falling edge)
    //MOVS    R0, #GPIO_BTN_DN            @ LED pin on PI PICO board
    //BL      asm_gpio_set_irq_rise       @ Call the subroutine (catch falling edge)

    MOVS    R0, #GPIO_BTN_EN            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_init               @ Call subroutine to init GPIO pin specified by R0
    MOVS    R0, #GPIO_BTN_EN            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_set_irq_fall           @ Call the subroutine (catch falling edge)
    MOVS    R0, #GPIO_BTN_EN            @ LED pin on PI PICO board
    BL      asm_gpio_set_irq_rise            @ Call the subroutine (catch falling edge)

    MOVS    R0, #GPIO_BTN_UP            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_init               @ Call the subroutine to init GPIO pin specified by R0
    MOVS    R0, #GPIO_BTN_UP            @ This value is the GPIO LED pin on the PI PICO board
    BL      asm_gpio_set_irq_fall           @ Call the subroutine (catch falling edge)
    //MOVS    R0, #GPIO_BTN_UP            @ LED pin on PI PICO board
    //BL      asm_gpio_set_irq_rise            @ Call the subroutine (catch falling edge)

    pop     {pc}                        @ Pop the link register from the stack to the program counter

set_alarm:
    ldr     r2, =TIMER_BASE             @ get timer base reg address
    movs    r1, #1                      @ set alarm
    str     r1, [r2, #TIMER_INTE_OFFSET]       @ enable timer interrupy
    ldr     r1, [r2, #TIMER_TIMELR_OFFSET]     @ get low time registers
    ldr     r3, =ltimer                 @ current flash interval address
    ldr     r0, [r3]                    @ get value of current flash interval
    add     r1, r0                      @ add delay
    str     r1, [r2, #TIMER_ALARM0_OFFSET]      @ set new alarm with delay
    bx      lr                          @ exit subtountine

install_alarm_isr:
@Installs alarm ISR, enables alarm0 irq
    LDR R2, =(PPB_BASE + M0PLUS_VTOR_OFFSET)
    LDR R1, [R2]
    MOVS R2, #ALRM_ISR_OFFSET
    ADD R2, R1

    LDR R0, =alrm_isr
    STR R0, [R2]

    MOVS R0, #1
    LDR R1, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)
    STR R0, [R1]

    LDR R1, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)
    STR R0, [R1]
    bx lr

.thumb_func
//installs the gpio isr
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

alrm_isr:
    push    {lr}
    ldr     r2, =TIMER_BASE         @ timer_base register address
    movs    r1, #1                  @ constant value to set alarm
    str     r1, [r2, #TIMER_INTR_OFFSET]    @ reset alarm
    ldr     r2, =atimer             @ address of variable
    movs    r0, #1                  @ constant value
    str     r0, [r2]                @ store constant value
    pop     {pc}

.thumb_func 
gpio_isr:
    push    {lr}
    bl      set_watchdog_update
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_PROC0_INTS2_OFFSET)      @ Read the interrupt status event from the (IO_BANK0_BASE + IO_BANK0_PROC0_INTS2_OFFSET) register
    ldr     r1, [r2]                @ acknowledge interrupt
    ldr     r0, =GPIO_BTN_DN_MSK    @ store mask in register
    cmp     r0, r1          @ compare interrupt with mask
    beq     button_levels_increment
    ldr     r0, =GPIO_BTN_ENF_MSK    @ mask for rising edge
    cmp     r0, r1
    beq     button_main_pressed
    ldr     r0, =GPIO_BTN_ENR_MSK    @ mask for rising edge
    cmp     r0, r1
    beq     button_main_released
    ldr     r0, =GPIO_BTN_UP_MSK    @ mask for rising edge
    cmp     r0, r1
    beq     button_flush_input
handle:
    //bl      start_timer         @ time to next interrupt
    pop     {pc}

.thumb_func 
//This is the gpio interrupt handler for the level select button
//will increment between the different level choices
button_levels_increment:
    bl      levels_increment
    ldr     r1, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    ldr     r0, [r1]
    ldr     r0, =GPIO_BTN_ENR_MSK    
    ldr     r2, [r1]
    orrs    r0, r0, r2          
    str     r0, [r1]
    ldr     r0, [r1]
    pop     {pc}

.thumb_func 
//This is the gpio interrupt handler for the main button

button_main_pressed:
    bl      start_timer
    ldr     r1, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    ldr     r0, [r1]
    ldr     r0, =GPIO_BTN_ENF_MSK     
    ldr     r2, [r1]
    orrs    r0, r0, r2          
    str     r0, [r1]
    ldr     r0, [r1]
    pop     {pc}

.thumb_func 
//Handles all of the morse code inputs as well as selevting the level
button_main_released:
    ldr     r1, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    ldr     r0, [r1]
    ldr     r0, =GPIO_BTN_ENR_MSK     
    ldr     r2, [r1]
    orrs    r0, r0, r2          
    str     r0, [r1]
    ldr     r0, [r1]
    bl      end_timer
    ldr     r1, =300000                      @ store 1 second
    cmp     r0, r1  
    bgt     add_dash 
    b       add_dot
    pop     {pc}

.thumb_func 
//Enter button for entering the user's input
button_flush_input:
    bl      flush_input
    ldr     r1, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    ldr     r0, [r1]
    ldr     r0, =GPIO_BTN_UP_MSK     
    ldr     r2, [r1]
    orrs    r0, r0, r2          
    str     r0, [r1]
    ldr     r0, [r1]
    pop     {pc}

falling_edge:
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)    @ address of INTR2 Register
    ldr     r1, =GPIO_BTN_DN_MSK        @ mask for gpio21 falling edge
    str     r1, [r2]                    @ reset gpio21 ISR
    bl      end_timer                   @ end timer
    movs    r1, #1                      @ store 1 second
    cmp     r0, r1                      @ check if one second passed
    bgt     add_space                   @ if greater than one second, it is a space
    b       add_dot                         @ if its less then one second, branch to dot
rising_edge:
    ldr     r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)     @ address of INTR2 Register
    ldr     r1, =GPIO_BTN_ENR_MSK        @ mask for gpio21 rising edge
    str     r1, [r2]                    @ reset gpio21 ISR
    bl      end_timer                   @ end timer
    movs    r1, #1                      @ store 1 second
    cmp     r0, r1                      @ check if one second passed
    bgt     add_dash                    @ if greater than one second, it is a dash
    b       handle                      @ else, restart timer
add_dot:
    movs    r0, #0                      @ number for dot is 0
    bl      add_input                   @ add user input
    b       handle
add_space:
    movs    r0, #2                      @ number for space is 2
    bl      add_input                   @ add user input
    b       handle
add_dash:
    movs    r0, #1                      @ number for dash is 1
    bl      add_input                   @ add user input
    b       handle

@ Set data alignment

.align 4
msg0:   .asciz "Use the GPIO 21 button to input your Morse Code Sequence."
msg1:   .asciz  "Use the GPIO 20 and GPIO 21 buttons to scroll up and down"
.data
ltimer:     .word   INPUT_END_TIME  @ store current alarm time
atimer:     .word   INITIAL_STATE   @init state of alarm
