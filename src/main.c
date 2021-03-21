#include <stdlib.h>
#include <stdio.h>
#include <control_lcd_16x2.h>
#include <bme280.h>
#include <uart.h>
#include <csv.h>
#include <pid.h>
#include <gpio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define SECOND 800000


int flag = 0;
int flag_imprime = 0;

void main_thread() {
    while(1) {
        float external = bme280_leTemperaturaExterna();
        float internal = read_temp(INTERNAL);
        float reference = read_temp(REFERENCE);

        if(internal < 0){
            usleep(SECOND);
            continue;
        }

        if(reference < 0) {
            usleep(SECOND);
            continue;
        }

		pid_atualiza_referencia(reference);
		double controle = pid_controle(internal);
		gpio_controle(controle);
        // printf("%f\n", controle);
        reference = retorna_referencia();
        lcdLoc(0x80);
        typeln("TI:");
        typeFloat(internal);
        typeln(" ");
        typeln("TE:");
        typeFloat(external);
        lcdLoc(0xC0);
        typeln("TR:");
        typeFloat(reference);
        if(flag_imprime == 1) {
            printf("\nTI: %4.2f \t TE: %4.2f \t TR: %4.2f", internal, external, reference);
        }
        
        if(flag == 1) {
            flag = 0;
        }
        else if(flag == 0) {
            escreve_CSV(internal, external, reference, controle);
            flag = 1;
        }


		usleep(SECOND);
    }
}

void quit_control() {
  power_off();
  exit(0);
}

void quit() {
  signal(SIGINT, quit_control);
}

void menu_thread() {
    while(1) {
        int option = 0;
        float reference = 0.0;
        // flag_imprime = 0;
        system("clear");
        printf("\nEscolha uma opção: \n");
        printf("1 - Atualizar referencia via terminal.\n");
        printf("2 - Atualizar referencia via potenciometro.\n");
        printf("3 - Visualizar dados dos sensores.\n");
        scanf("%d", &option);

        switch(option) {
            case 1:
                printf("Digite o novo valor de referencia: \n");
                scanf("%f", &reference);
                pid_referencia_menu(reference);
                break;
            case 2: 
                pid_limpa_referencia_menu();
                break;
            case 3:
                system ("tput clear");
                flag_imprime = 1;
                printf("Pressione [ENTER] para voltar ao menu anterior!");
                while(getchar()!='\n');
                getchar();
                flag_imprime = 0;
                // break;

        }
    }
}

int main(int argc, const char * argv[]) {
    lcd_init();
    ClrLcd();
    inicializa_bme280();
    gpio_init();
    cria_CSV();
    quit();

	pthread_t thread_id[2];
    pthread_create(&thread_id[0], NULL, (void *) main_thread, (void *) NULL);
    pthread_create(&thread_id[1], NULL, (void *) menu_thread, (void *) NULL);
	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
    
    
    return 0;
}
