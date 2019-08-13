//led PD12,13,14,15
#include "stm32f407xx.h"
#include <__cross_studio_io.h>
//DMA2 channel 1 stream 0

//note
//if ARPE = 0 ARR is updated immediately
//if ARPE = 1 ARR is updated on an event

void timer_led_init();
void dma_init();

int main()
{
    timer_led_init();
    dma_init();

    DMA2_Stream0->CR |= (1<<0); //start dma transfer
    while(((DMA2->LISR) & (1<<5)) == 0); //wait for transfer complete
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

void timer_led_init(){
    RCC -> APB1ENR |= (1<<4); //timer 6 (basic) clock enable
    RCC -> AHB1ENR |= (1<<3); //gpio d clock enable
    GPIOD ->MODER |= (1<<26);  //gpio mode selected
    GPIOD -> PUPDR |= (1<<27); //pull down

    TIM6 -> CNT = 5000;
    TIM6 -> PSC = 255; //prescaler
   
}

void dma_init(){
    RCC->AHB1ENR |=(1<<22);
    DMA2->LIFCR |= (1<<5); //clear complete transfer flag
    DMA2_Stream0->CR = 0x00000000; //clear enable pin
    while((DMA2_Stream0->CR) & (1<<0)); //wait to be cleared
    //byte to half word,peripheral_pointer_increment, m2m, Transfer_complete_interrupt_enable, channel1
    DMA2_Stream0->CR |= (1<<13)|(1<<9)|(1<<7)|(1<<4)|(1<<25);
    DMA2_Stream0->NDTR = 2; //2 data item
    DMA2_Stream0->PAR = 0x40001024; //from address tim6_cnt reg
    DMA2_Stream0->M0AR = 0x4000102C; //to address tim6_arr reg
}