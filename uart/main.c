#include <__cross_studio_io.h>
//led PD12,13,14,15
//usart1 tx:PB6 rx:PB7
#include "stm32f407xx.h"
void usart_init();
void delay();

int data = 0;

int main()
{
    usart_init();
    //led init
    RCC -> AHB1ENR |= (1<<3);
    GPIOD ->MODER |= (1<<26);
    GPIOD -> PUPDR |= (1<<27);

    while(1){
        //while ((USART1->SR & (1 << 7)) == 0);
        //USART1->DR = 1;
        //while ((USART1->SR & (1 << 6)) == 0);
        while((USART1->SR & (1 << 5)) == 0);
        data = USART1->DR;
        delay();
        USART1->SR &= ~(1 << 5);
        if(data == 49){
          GPIOD ->ODR |= (1<<13);
          delay();
          GPIOD ->ODR = 0x00000000;
        }
    }
    return 0;
}

void delay(){
   for(int i =0 ; i<=1000;i++)
    for(int i =0 ; i<=100;i++);
}

void usart_init(){
    RCC -> AHB1ENR |= (1<<1); //port B clock enable 
    GPIOB ->MODER |= ((1<<13) | (1<<15)); //set port as alternate function
    GPIOB ->MODER &= ~((1<<12)|(1<<14));
    GPIOB ->OTYPER |= (1<<6)|(1<<7); 
    GPIOB->PUPDR |= ((1<<12)|(1<<14));
    GPIOB ->AFR[0] = 0x77000000; //set portA for usart function
    RCC -> APB2ENR |= (1<<4); //usart1 clock enable

    USART1 ->CR1 |= (1<<13); //turn on uart
    USART1->BRR = 0x0683; //9.6kbps 16MHz
    USART1 ->CR1 |= ((1<<3) | (1<<2)); //turn tx rx on

    for(int i =0 ;i<=1000;i++);  //small delay
}
