ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    DTCMRAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);

SECTIONS
{
    /* Interrupt vector table */
    .isr_vector :
    {
        . = ALIGN(1024);
        KEEP(*(.isr_vector))
    } > FLASH

    /* Program instructions and constants */
    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* ARM exception index */
    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH

    /* Initialized global and static variables */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > DTCMRAM AT > FLASH

    _sidata = LOADADDR(.data);

    /* Uninitialized global and static variables */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > DTCMRAM

    /* Heap and stack reservation */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        . = . + 0x400;
        . = ALIGN(8);
    } > DTCMRAM
}