#include <wiringPi.h>
#include <stdio.h>    
#include <softPwm.h>

#define LIGADO 100
#define DESLIGADO 0

#define RESISTOR 4
#define VENTOINHA 5

#ifndef GPIO_H_
#define GPIO_H_

void gpio_init();
void gpio_controle(double controle);
void toggle(int item, int status);
void power_off();

#endif /* GPIO_H_ */