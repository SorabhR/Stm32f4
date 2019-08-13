//led PD12,13,14,15
#include "stm32f407xx.h"

void rtc_init();
void delay();

int time = 0; 

int main()
{
  rtc_init();
  while(1){
    while(((RTC->ISR) & (1<<5)) == 0); //RSF poll
    time = RTC->DR;
    RTC->ISR &= ~(1<<5);//RSF clear
}
}

void delay(){
   for(int i =0 ; i<=1000;i++)
    for(int j =0 ; j<=100;j++);
}


void rtc_init(){
  RCC->APB1ENR |= (1<<28); //clk for pwr reg
  PWR->CR |=  (1<<8); //set dbp bit for rtc reg access
  RCC->BDCR = 0;
  RCC->CSR |= (1<<0); //LSI on
  while(((RCC->CSR) & (1<<1)) == 0);//LSI RDY
  RCC->BDCR |= (1<<9)|(1<<15); //set rtc clk source, rtc clk enable

  RTC->WPR = 0xCA; //psw for rtc
  RTC->WPR = 0x53; //psw for rtc

  RTC->ISR |= (1<<7); //INIT set
  while(((RTC->ISR) & (1<<6)) == 0); //INITF poll
  RTC->PRER |= 0x000000FF; //sync prescaler
  RTC->PRER |= 0x007F0000; //async prescaler
  
  RTC->TR = (RTC_TR_HT_0)|(RTC_TR_HU_0)|(RTC_TR_MNT_2); //11:40:00
  RTC->DR = 0;
  RTC->DR = (RTC_DR_YT_0)|(RTC_DR_YU_3)|(RTC_DR_YU_0)|(RTC_DR_MU_3)|(RTC_DR_DU_3);//  8/8/19

  RTC->CR = 0; //BYPSHAD = 0, 24-HR format

  RTC->ISR &= ~(1<<7);//INIT clear
  RTC->ISR &= ~(1<<5);//RSF clear
  while(((RTC->ISR) & (1<<4)) == 0); //INITS poll
}