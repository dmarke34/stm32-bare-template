# stm32-bare-template

Bare-metal STM32F767ZI starting point: hand-written linker script, startup code, and register-level blinky. No HAL, no IDE.

## Build and flash
    make
    make flash

## Memory layout

1. Where does .isr_vector start, and why does it have to be there?

.isr_vector starts at 0x08000000, the beginning of flash, which the Cortex-M7 is looking at on reset. It loads word 0 into the stack pointer (0x20080000) and jumps to the address in word 1 (Reset_Handler). 

2. How big is .text, and which functions are in it? 

.text is 216 bytes (0xD8), from 0x08000040 to 0x08000118, right after the vector table. It holds main (96 bytes), Reset_Handler (116 bytes), and Default_Handler (2 bytes). All the unused exception handlers are aliases that share Default_Handler’s address. delay() doesn’t appear because the compiler inlined it into main.

3. Why are .data and .bss both 0 bytes?

.data and .bss are both 0 bytes because blinky has no global variables. The register names and pin numbers are #define macros, count in delay() is a local on the stack, and vector_table is const, so it stays in flash. In the nm output, _sdata, _edata, _sbss and _ebss are all 0x20000000: each section starts and ends at the same address. Reset_Handler’s copy and zero loops still run, but they do zero iterations.

4. What is _estack, and where did you see that number?

_estack is 0x20080000, the top of RAM, calculated in the linker script as ORIGIN(RAM) + LENGTH(RAM). It’s the first entry of the vector table, so on reset the chip loads it into the main stack pointer, and the stack then grows down from there. make flash confirmed it: OpenOCD reported msp: 0x20080000 when it halted the CPU.