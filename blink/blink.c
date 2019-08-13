//led PD12,13,14,15
#include "stm32f407xx.h"

void delay();

int main()
{
    RCC -> AHB1ENR |= (1<<3);
    GPIOD ->MODER |= (1<<26);
    GPIOD -> PUPDR |= (1<<27);
    while(1){
        GPIOD ->ODR = 0x00000000;
        delay();
        GPIOD ->ODR |= (1<<13);
        delay();
    }
    return 0;
}

void delay(){
    int i,j,k = 0;
    for(i = 0;i<=5000;i++)   
        for(j = 0;j<=500;j++);
}