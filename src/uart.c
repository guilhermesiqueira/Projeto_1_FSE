#include "uart.h"
#include "crc16.h"

int openUART() {
  int uart0_filestream = -1;

  uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);  //Open in non blocking read/write mode
  if (uart0_filestream == -1) {
    printf("Erro - Não foi possível iniciar a UART.\n");
    exit(1);
  }

  struct termios options;
  tcgetattr(uart0_filestream, &options);
  options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;  // < Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_filestream, TCIFLUSH);
  tcsetattr(uart0_filestream, TCSANOW, &options);

  return uart0_filestream;
}

float readUART(int uart0_filestream) {
  unsigned char rx_buffer[256];
  if (uart0_filestream != -1) {
    int rx_length = read(uart0_filestream, (void*)rx_buffer, 255); // Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length < 0) {
      printf("Erro na leitura.\n"); // An error occured (will occur if there are no bytes)
      return -1;
    }
    else if (rx_length == 0) {
      printf("Nenhum dado disponível.\n"); // No data waiting
      return -1;
    }
    // else {
    //   // Bytes received
    //   rx_buffer[rx_length] = '\0';
    // }

    short crc;
    memcpy(&crc, &rx_buffer[7], 2);

    unsigned char package[7];
    memcpy(&package, &rx_buffer[0], 7);
    if (crc == calcula_CRC(package, 7)) {
        float temperature;
        memcpy(&temperature, &rx_buffer[3], 4);
        return temperature;
    }
  }

  return -1;
}

void writeUART(int uart0_filestream, char temp_type) {
  
  unsigned char tx_buffer[20] = {
      0x01,
      0x23,
      temp_type,
      0x07,
      0x07,
      0x06,
      0x03
  };

  short crc = calcula_CRC(tx_buffer, 7);
  memcpy(&tx_buffer[7], &crc, 2);

  if (uart0_filestream != -1) {
    int count = write(uart0_filestream, &tx_buffer, 9);
    if (count < 0) {
      printf("UART write error.\n");
      exit(1);
    }
  }
}

float read_temp(char temp_type) {
  int uart0_filestream;

  uart0_filestream = openUART();

  writeUART(uart0_filestream, temp_type);

  usleep(100000);

  float temperature = readUART(uart0_filestream);

  close(uart0_filestream);

  return temperature;
}