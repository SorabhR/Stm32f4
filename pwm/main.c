#include <__cross_studio_io.h>
//led PD12,13,14,15
//timer4
#include "stm32f407xx.h"
void timer_init();
void delay();

int data = 0;

int main()
{
    timer_init();
    TIM4->CCR2 = 2000;
    TIM4->CCMR1 |= (1<<11); //auto preload enable
    while(1);
    return 0;
}

void delay(){
   for(int i =0 ; i<=1000;i++)
    for(int i =0 ; i<=10000;i++);
}

void timer_init(){
    //led init d13
    RCC -> AHB1ENR |= (1<<3);
    GPIOD ->MODER |= (1<<27);
    GPIOD ->AFR[1] = 0x00200000; //set D13 for pwm function

    //timer
    RCC -> APB1ENR |= (1<<2); //timer4 clock enable
    TIM4->ARR = 65535;
    TIM4->CR1 |= (1<<0); //counter enabled

    //using pwm channel 2 mode 2
    TIM4->CCMR1 |= (1<<12)|(1<<13)|(1<<14); //selecting pwm mode 2 and set oc2 as o/p
    TIM4->CCER |= (1<<4)|(1<<5); //OC2 signal is output on the corresponding output pin and setting active low
}
