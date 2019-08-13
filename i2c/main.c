#include <__cross_studio_io.h>
//led PD12,13,14,15
//i2c1 scl:PB6 sda:PB7
//master 100KHz transmit and read
#include "stm32f407xx.h"
void i2c_init();
void delay();

void send_start();
void send_stop();
void send_data(uint8_t data);

void recieve_data(int bytes_no);


uint8_t addr = 0x08;
int data_rec[5]= {0};
int index = 0;


int main()
{
    i2c_init();
    //led init
    RCC -> AHB1ENR |= (1<<3);
    GPIOD ->MODER |= (1<<26);
    GPIOD -> PUPDR |= (1<<27);

//send 6 and then 5
//    send_data(6);
//    delay();
//    send_data(6);
//    send_stop();

    int no_bits = 3;
    recieve_data(no_bits);
    delay();

    for (int i = 0;i<no_bits;i++)
      send_data(data_rec[i]);
    send_stop();

    while(1);
    return 0;
}

void delay(){
   for(int i =0 ; i<=1000;i++)
    for(int i =0 ; i<=100;i++);
}

void i2c_init(){
    RCC -> AHB1ENR |= (1<<1); //port B clock enable 
    GPIOB ->MODER |= ((1<<13) | (1<<15)); //set port as alternate function
    GPIOB ->MODER &= ~((1<<12)|(1<<14));
    GPIOB ->OTYPER |= (1<<6)|(1<<7); 
    GPIOB->PUPDR |= ((1<<12)|(1<<14));
    GPIOB ->AFR[0] = 0x44000000; //set portB for i2c function
    RCC -> APB1ENR |= (1<<21); //i2c1 clock enable


    I2C1->CR1 |= (1<<10);//ack enable
    I2C1->CR2 |= (1<<2)|(1<<9)|(1<<10); //8MHz peripheral clock and interrupt enable
    //100KHz
    I2C1->CCR = 0x0028;
    I2C1->TRISE = 0x0009;
    I2C1->CR1 |= (1<<0);//peripheral enable


    for(int i =0 ;i<=1000;i++);  //small delay
}


void send_start(){
    I2C1->CR1 |= (1<<8); //start condition
    while(((I2C1->SR1) & (1<<0)) == 0);
}

void send_stop(){
    I2C1->CR1 |= (1<<9); //stop condition
}

void send_data(uint8_t data){
    send_start();
    I2C1->DR = (addr<<1); //address transmission
    while(((I2C1->SR1) & (1<<1)) == 0); //wait for ADDR bit
    while(((I2C1->SR2) & (1<<2)) == 0); //wait for TRA bit
    while(((I2C1->SR1) & (1<<7)) == 0); //wait for tx buffer to be empty
    I2C1->DR = data; 
    while(((I2C1->SR1) & (1<<2)) == 0); //wait for transmission to complete
}

void recieve_data(int bytes_no){
    send_start();
    if (bytes_no == 1){
        I2C1->DR = (addr<<1)|(1<<0); //address transmission
        while(((I2C1->SR1) & (1<<1)) == 0); //wait for ADDR bit
        I2C1->CR1 &= ~(1<<10); //clear ack
        while(((I2C1->SR1) & (1<<6)) == 0);
        send_stop();
        data_rec[index] = I2C1->DR;
        I2C1->CR1 |= (1<<10);//ack enable
    }

    else if (bytes_no == 2){
        I2C1->CR1 &= ~(1<<10); //clear ack
        I2C1->CR1 |= (1<<11); //set POS
        I2C1->DR = (addr<<1)|(1<<0); //address transmission
        while(((I2C1->SR1) & (1<<1)) == 0); //wait for ADDR bit
        while(((I2C1->SR1) & (1<<2)) == 0); //wait for BTF = 1
        
        send_stop();
        
        data_rec[index] = I2C1->DR;
        index++;
        data_rec[index] = I2C1->DR;

        I2C1->CR1 |= (1<<10);//ack enable
    }

    else{
      I2C1->CR1 |= (1<<10);//ack enable
      I2C1->DR = (addr<<1)|(1<<0); //address transmission
      while(((I2C1->SR1) & (1<<1)) == 0); //wait for ADDR bit
      
      while(bytes_no - 3 > 0){
        while(((I2C1->SR1) & (1<<6)) == 0); //wait for RxNE = 1
        data_rec[index] = I2C1->DR;
        index++;
        bytes_no--;
      }
      
      while(((I2C1->SR1) & (1<<2)) == 0); //wait for BTF = 1
      I2C1->CR1 &= ~(1<<10); //clear ack
      data_rec[index] = I2C1->DR;
      index++;
      while(((I2C1->SR1) & (1<<2)) == 0); //wait for BTF = 1
      send_stop();
      data_rec[index] = I2C1->DR;
      index++;
      data_rec[index] = I2C1->DR;
    }
}