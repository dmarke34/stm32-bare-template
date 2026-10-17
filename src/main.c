#include <stdint.h>

/* Addresses from RM0410: memory map (section 2.2.2), RCC (section 5.3), GPIO (section 6.4) */
#define RCC_BASE      0x40023800u
#define GPIOB_BASE    0x40020400u

#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE   + 0x30u))
#define GPIOB_MODER   (*(volatile uint32_t *)(GPIOB_BASE + 0x00u))
#define GPIOB_BSRR    (*(volatile uint32_t *)(GPIOB_BASE + 0x18u))

#define GPIOBEN       (1u << 1)
#define LED1_PIN      0u          /* LD1 green = PB0 on Nucleo-F767ZI */

static void delay(volatile uint32_t count)
{
    while (count--) { }
}

int main(void)
{
    RCC_AHB1ENR |= GPIOBEN;       /* turn on the GPIOB clock */
    (void)RCC_AHB1ENR;            /* read back: gives the clock time to start */

    GPIOB_MODER &= ~(3u << (LED1_PIN * 2u));   /* clear mode bits */
    GPIOB_MODER |=  (1u << (LED1_PIN * 2u));   /* 01 = general-purpose output */

    while (1) {
        GPIOB_BSRR = (1u << LED1_PIN);            /* set PB0: LED on */
        delay(800000u);
        GPIOB_BSRR = (1u << (LED1_PIN + 16u));    /* reset PB0: LED off */
        delay(800000u);
    }
}