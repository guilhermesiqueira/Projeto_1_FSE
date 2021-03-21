#include <gpio.h>

void gpio_init() {
  wiringPiSetup();
}

void toggle(int item, int status) {
  pinMode(item, OUTPUT);
  softPwmCreate(item, 1, LIGADO);
  softPwmWrite(item, status);
}

void gpio_controle(double controle) {
  if (controle > 0) {
    toggle(VENTOINHA, DESLIGADO);
    toggle(RESISTOR, controle);
  } else if (controle < -40) {
    toggle(RESISTOR, DESLIGADO);
    toggle(VENTOINHA, controle * -1);
  } else {
    power_off();
  }
}

void power_off() {
  pinMode(RESISTOR, OUTPUT);
  softPwmWrite(RESISTOR, DESLIGADO);

  pinMode(VENTOINHA, OUTPUT);
  softPwmWrite(VENTOINHA, DESLIGADO);
}