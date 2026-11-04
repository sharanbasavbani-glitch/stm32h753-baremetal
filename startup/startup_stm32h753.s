.syntax unified
.cpu cortex-m7
.thumb

.global Reset_Handler
.global Default_Handler

.extern main
.extern _estack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss

.section .isr_vector, "a", %progbits
.align 10

.word _estack
.word Reset_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word 0
.word 0
.word 0
.word 0
.word Default_Handler
.word Default_Handler
.word 0
.word Default_Handler
.word Default_Handler

/* External interrupt vector entries: IRQ 0-149 */
.rept 150
    .word Default_Handler
.endr

.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function

Reset_Handler:

    /* Copy initialized data into RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcs zero_bss_start
    ldr r3, [r0], #4
    str r3, [r1], #4
    b copy_data

zero_bss_start:

    /* Clear BSS section */
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0

zero_bss:
    cmp r1, r2
    bcs call_main
    str r3, [r1], #4
    b zero_bss

call_main:

    bl main

infinite_loop:
    b infinite_loop

.size Reset_Handler, .-Reset_Handler

.section .text.Default_Handler
.weak Default_Handler
.type Default_Handler, %function

Default_Handler:
    b Default_Handler

.size Default_Handler, .-Default_Handler