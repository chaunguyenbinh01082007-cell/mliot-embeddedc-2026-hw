#include "stm32f103xb.h"

void GPIO_4Btn_4Led_Init(void);
void GPIO_4Btn_4Led_Handler(void);

int main(void)
{
    GPIO_4Btn_4Led_Init();

    while (1)
    {
        GPIO_4Btn_4Led_Handler();
    }
}

void GPIO_4Btn_4Led_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    
    volatile uint32_t dummy = RCC->APB2ENR;
    (void)dummy;

    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG) | AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1 | GPIO_CRL_MODE2 | GPIO_CRL_CNF2 | 
                    GPIO_CRL_MODE3 | GPIO_CRL_CNF3 | GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOA->CRL |= (GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1);
    GPIOA->ODR |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);

    GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0 | GPIO_CRL_MODE1 | GPIO_CRL_CNF1 | 
                    GPIO_CRL_MODE3 | GPIO_CRL_CNF3 | GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOB->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE1_1 | GPIO_CRL_MODE3_1 | GPIO_CRL_MODE4_1);
    GPIOB->ODR &= ~((1 << 0) | (1 << 1) | (1 << 3) | (1 << 4));
}

void GPIO_4Btn_4Led_Handler(void)
{
    static uint8_t btn1_last = 1, btn2_last = 1, btn3_last = 1, btn4_last = 1;
    uint8_t btn1_curr = (GPIOA->IDR & (1 << 1)) ? 1 : 0;
    uint8_t btn2_curr = (GPIOA->IDR & (1 << 2)) ? 1 : 0;
    uint8_t btn3_curr = (GPIOA->IDR & (1 << 3)) ? 1 : 0;
    uint8_t btn4_curr = (GPIOA->IDR & (1 << 4)) ? 1 : 0;

    if (btn1_curr == 0 && btn1_last == 1)
    {
        for (volatile int i = 0; i < 30000; i++);
        if (!(GPIOA->IDR & (1 << 1))) GPIOB->ODR ^= (1 << 0);
    }
    btn1_last = btn1_curr;

    if (btn2_curr == 0 && btn2_last == 1)
    {
        for (volatile int i = 0; i < 30000; i++);
        if (!(GPIOA->IDR & (1 << 2))) GPIOB->ODR ^= (1 << 1);
    }
    btn2_last = btn2_curr;

    if (btn3_curr == 0 && btn3_last == 1)
    {
        for (volatile int i = 0; i < 30000; i++);
        if (!(GPIOA->IDR & (1 << 3))) GPIOB->ODR ^= (1 << 3);
    }
    btn3_last = btn3_curr;

    if (btn4_curr == 0 && btn4_last == 1)
    {
        for (volatile int i = 0; i < 30000; i++);
        if (!(GPIOA->IDR & (1 << 4))) GPIOB->ODR ^= (1 << 4);
    }
    btn4_last = btn4_curr;
}