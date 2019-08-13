//led PD12,13,14,15
#include "stm32f407xx.h"

//note
//if ARPE = 0 ARR is updated immediately
//if ARPE = 1 ARR is updated on an event

int main()
{
    RCC -> APB1ENR |= (1<<4); //timer 6 (basic) clock enable
    RCC -> AHB1ENR |= (1<<3); //gpio d clock enable
    GPIOD ->MODER |= (1<<26);  //gpio mode selected
    GPIOD -> PUPDR |= (1<<27); //pull down

    TIM6 -> ARR = 65535; //timer counts from 0 to this value
    TIM6 -> PSC = 255; //prescaler
    TIM6 -> CR1 |= (1<<0); //enable counter
    while(1){
        GPIOD ->ODR = 0x00000000;
        TIM6 -> SR &= ~(1<<0); //clear overflow flag
        while (!(TIM6 -> SR && 0x00000001)); //check overflow
        GPIOD ->ODR |= (1<<13);
        TIM6 -> SR &= ~(1<<0);
        while (!(TIM6 -> SR && 0x00000001));
    }
    return 0;
}