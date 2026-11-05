#include <stdint.h>
#define RCC_AHB4ENR (*(volatile uint32_t *) 0x580244E0)
#define GPIOB_MODER (*(volatile uint32_t *) 0x58020400)
#define GPIOB_PUPDR (*(volatile uint32_t *) 0x5802040C)
#define GPIOB_IDR (*(volatile uint32_t *) 0x58020410)
#define GPIOB_ODR (*(volatile uint32_t *) 0x58020414)
#define GPIOB_BSRR (*(volatile uint32_t *) 0x58020418)

#define GPIOB_CLOCK_ENABLE (1U << 1)
static void delay(void)
{
    for (volatile uint32_t i = 0; i < 800000U; i++)
    {
        /* Software delay */
    }
}

int main(void)
{
	RCC_AHB4ENR |= GPIOB_CLOCK_ENABLE;
	(void)RCC_AHB4ENR;
	
	GPIOB_MODER &= ~(3u<<0);
	GPIOB_MODER |= (1u<<0);

	GPIOB_MODER &= ~(3u<<6);
	
	
	GPIOB_PUPDR &= ~(3u<<6);
	GPIOB_PUPDR |= (1u<<6);

	GPIOB_BSRR = (1u<<16); /* intially off */ 
	 
	delay();

	while(1)
	{
	if (GPIOB_IDR & (1u<<3))
	{	
		GPIOB_BSRR = (1u<<16);
		delay();
	}
	else
	{
		delay();
		delay();
		if(GPIOB_IDR & (1U<<3))
		{
			GPIOB_BSRR = (1u<<16);
		}
		else 
		{
			GPIOB_BSRR = (1u<<0);
		}
			
		delay();
	}
	}
	
	
	
}

