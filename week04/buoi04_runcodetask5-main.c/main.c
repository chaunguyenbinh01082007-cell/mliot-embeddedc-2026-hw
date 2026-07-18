#include "stm32f103xb.h"

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 4000; i++);
}

void delay_us(uint32_t us) {
    for (volatile uint32_t i = 0; i < us * 4; i++);
}

int main(void) {
    RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 3);
    AFIO->MAPR = (AFIO->MAPR & ~(7 << 24)) | (2 << 24);

    GPIOA->CRL &= ~(0x000FFFF0);
    GPIOA->CRL |=  (0x00088220);
    GPIOA->ODR |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);

    GPIOB->CRL &= ~(0x000FF0FF);
    GPIOB->CRL |=  (0x00022022);
    GPIOB->ODR &= ~((1 << 0) | (1 << 1) | (1 << 3) | (1 << 4));

    GPIOB->ODR |= ((1 << 0) | (1 << 1) | (1 << 3) | (1 << 4));
    delay_ms(1000);
    GPIOB->ODR &= ~((1 << 0) | (1 << 1) | (1 << 3) | (1 << 4));
    delay_ms(200);

    uint8_t btn_state[4] = {1, 1, 1, 1};
    uint8_t last_btn_state[4] = {1, 1, 1, 1};

    while (1) {
        GPIOA->ODR &= ~(1 << 1);
        delay_us(10);
        btn_state[0] = (GPIOA->IDR & (1 << 3)) ? 1 : 0;
        btn_state[1] = (GPIOA->IDR & (1 << 4)) ? 1 : 0;
        GPIOA->ODR |= (1 << 1);

        GPIOA->ODR &= ~(1 << 2);
        delay_us(10);
        btn_state[2] = (GPIOA->IDR & (1 << 3)) ? 1 : 0;
        btn_state[3] = (GPIOA->IDR & (1 << 4)) ? 1 : 0;
        GPIOA->ODR |= (1 << 2);

        if (last_btn_state[0] == 1 && btn_state[0] == 0) {
            GPIOB->ODR ^= (1 << 0);
            delay_ms(50);
        }
        if (last_btn_state[1] == 1 && btn_state[1] == 0) {
            GPIOB->ODR ^= (1 << 1);
            delay_ms(50);
        }
        if (last_btn_state[2] == 1 && btn_state[2] == 0) {
            GPIOB->ODR ^= (1 << 3);
            delay_ms(50);
        }
        if (last_btn_state[3] == 1 && btn_state[3] == 0) {
            GPIOB->ODR ^= (1 << 4);
            delay_ms(50);
        }

        for (int i = 0; i < 4; i++) {
            last_btn_state[i] = btn_state[i];
        }

        delay_ms(20);
    }
}