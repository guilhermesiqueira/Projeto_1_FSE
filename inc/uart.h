#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Used for UART
#include <fcntl.h>   // Used for UART
#include <termios.h> // Used for UART
#include <stdint.h>
#define INTERNAL 0xC1
#define REFERENCE 0xC2
#ifndef UART_H_
#define UART_H_

float read_temp(char temp_type);

#endif /* UART_H_ */
