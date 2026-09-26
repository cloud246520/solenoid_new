#ifndef _REGISTER_H_
#define _REGISTER_H_

/* includes ----------------------------------------------------*/
#include <main.h>

/* defines -----------------------------------------------------*/
// PA0 = SCK, PA1 = RCK, PA2 = SI
#define SI_GPIO_PORT        GPIOA
#define SI_PIN              GPIO_PIN_2

#define SCK_GPIO_PORT       GPIOA
#define SCK_PIN             GPIO_PIN_0

#define RCK_GPIO_PORT       GPIOA
#define RCK_PIN             GPIO_PIN_1


/* fuction -----------------------------------------------------*/
void register_init(void);
void solenoid_update(uint8_t controlvalue);
void register_clear(void);

#endif 
