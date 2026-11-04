#include <stdint.h>

/* RCC registers */
#define RCC_AHB4ENR   (*(volatile uint32_t *)0x580244E0U)

/* GPIOB registers */
#define GPIOB_MODER  (*(volatile uint32_t *)0x58020400U)
#define GPIOB_BSRR   (*(volatile uint32_t *)0x58020418U)

#define GPIOB_CLOCK_ENABLE (1U << 1)
#define LED_PIN            (1U << 0)

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 800000U; i++)
    {
        /* Software delay */
    }
}

int main(void)
{
    /* Enable GPIOB peripheral clock */
    RCC_AHB4ENR |= GPIOB_CLOCK_ENABLE;

    /* Read back RCC register */
    (void)RCC_AHB4ENR;

    /* Configure PB0 as output */
    GPIOB_MODER &= ~(3U << 0);
    GPIOB_MODER |=  (1U << 0);

    while (1)
    {
        /* Turn ON Green LED */
        GPIOB_BSRR = LED_PIN;
        delay();

        /* Turn OFF Green LED */
        GPIOB_BSRR = (LED_PIN << 16);
        delay();
    }
}