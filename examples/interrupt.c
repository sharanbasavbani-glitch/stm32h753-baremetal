#include <stdint.h>
#define REG32(addr) (*(volatile uint32_t *)(uintptr_t)(addr))

#define RCC_AHB4ENR       REG32(0x580244E0U)
#define RCC_APB4ENR       REG32(0x580244F4U)

#define GPIOB_MODER       REG32(0x58020400U)
#define GPIOB_OTYPER      REG32(0x58020404U)
#define GPIOB_PUPDR       REG32(0x5802040CU)
#define GPIOB_BSRR        REG32(0x58020418U)

#define SYSCFG_EXTICR1    REG32(0x58000408U)

#define EXTI_RTSR1        REG32(0x58000000U)
#define EXTI_FTSR1        REG32(0x58000004U)
#define EXTI_CPUIMR1      REG32(0x58000080U)
#define EXTI_CPUPR1       REG32(0x58000088U)

#define NVIC_ISER0        REG32(0xE000E100U)

#define LED_PIN           0U
#define BUTTON_PIN        3U

#define BUTTON_MASK       (1U << BUTTON_PIN)

#define EXTI3_IRQ_NUMBER  9U
volatile uint32_t button_event = 0U;

void GPIO_Init(void)
{
    RCC_AHB4ENR |= (1U << 1);
    GPIOB_MODER &= ~((3U << (LED_PIN * 2U)) |
                    (3U << (BUTTON_PIN * 2U)));

    GPIOB_MODER |= (1U << (LED_PIN * 2U));

    GPIOB_OTYPER &= ~(1U << LED_PIN);

    GPIOB_PUPDR &= ~(3U << (BUTTON_PIN * 2U));
    GPIOB_PUPDR |=  (1U << (BUTTON_PIN * 2U));

    GPIOB_BSRR = (1U << (LED_PIN + 16U));
}

void EXTI_Init(void)
{
    RCC_APB4ENR |= (1U << 1);

    SYSCFG_EXTICR1 &= ~(0xFU << 12);
    SYSCFG_EXTICR1 |=  (0x1U << 12);

    EXTI_CPUIMR1 &= ~BUTTON_MASK;


    EXTI_FTSR1 |= BUTTON_MASK;

    EXTI_CPUPR1 = BUTTON_MASK;

    EXTI_CPUIMR1 |= BUTTON_MASK;

void NVIC_Init(void)
{

    NVIC_ISER0 = (1U << EXTI3_IRQ_NUMBER);
}

void EXTI3_IRQHandler(void)
{
    if (EXTI_CPUPR1 & BUTTON_MASK)
    {
        button_event = 1U;

        EXTI_CPUPR1 = BUTTON_MASK;
    }
}


int main(void)
{
    GPIO_Init();

    EXTI_Init();

    NVIC_Init();

    while (1)
    {
        if (button_event != 0U)
        {
            button_event = 0U;

            GPIOB_BSRR = (1U << LED_PIN);
        }
    }
}
