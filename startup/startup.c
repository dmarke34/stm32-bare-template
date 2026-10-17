#include <stdint.h>

/* Symbols defined in the linker script */
extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;

int main(void);
void Reset_Handler(void);
void Default_Handler(void);

/* Weak aliases: any handler you don't write falls through to Default_Handler */
void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

/* Vector table: entry 0 is the initial stack pointer, entry 1 is the reset vector */
__attribute__((section(".isr_vector"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))(&_estack),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,               /* reserved */
    SVC_Handler,
    DebugMon_Handler,
    0,                        /* reserved */
    PendSV_Handler,
    SysTick_Handler,
    /* peripheral IRQs (USART3, TIM2, ...) get added here in week 2 */
};

#define SCB_CPACR (*(volatile uint32_t *)0xE000ED88u)

void Reset_Handler(void)
{
    /* 1. Enable the FPU (CP10 + CP11 full access), since we build with -mfloat-abi=hard */
    SCB_CPACR |= (0xFu << 20);

    /* 2. Copy .data initial values from flash to RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* 3. Zero .bss */
    for (dst = &_sbss; dst < &_ebss; ) {
        *dst++ = 0u;
    }

    /* 4. Call main; trap if it ever returns */
    main();
    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }   /* set a breakpoint here to catch unexpected interrupts */
}