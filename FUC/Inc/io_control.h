#ifndef _io_control_H_
#define _io_control_H_


#include<main.h>

#define IO_CH1_PORT GPIOA
#define IO_CH1_PIN  GPIO_PIN_11
#define IO_CH2_PORT GPIOA
#define IO_CH2_PIN  GPIO_PIN_12
#define IO_CH3_PORT GPIOB
#define IO_CH3_PIN  GPIO_PIN_6
#define IO_CH4_PORT GPIOB
#define IO_CH4_PIN  GPIO_PIN_7

void single_channel_on(uint8_t cmd);  //单个通道开启
void io_control(uint8_t cmd);

#endif
