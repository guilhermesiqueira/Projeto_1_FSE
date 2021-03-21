#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "csv.h"

char buffer[10];

void cria_CSV() {
    FILE *file = fopen("temperatures.csv", "w");
    fprintf(file, "DATA_HORA, TI, TE, TR, ATUADOR\n");
    fclose(file);
}
void escreve_CSV(float internal, float external, float reference, float atuador){
    time_t current_time;
    struct tm* time_info;

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(buffer, 10, "%H:%M:%S", time_info);

    FILE *file = fopen("temperatures.csv", "a");
    fprintf(file, "%s,%4.2f,%4.2f,%4.2f,%4.2f\r\n",buffer, internal, external, reference, atuador);
    fclose(file);
}
