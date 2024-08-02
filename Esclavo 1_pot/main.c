//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega238p
// Creado: 26/07/2024
//Última modificación: 29/07/2024
//******************************************************************************
// CODIGO DEL ESCLAVO QUE TIENE EL POTENCIÓMETRO

#define F_CPU 16000000UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADC/ADC.h"
#include "UART/UART.h"


////////////////////////////////////////////////////
// Variables.
////////////////////////////////////////////////////
char buffer[64];  // Buffer para las cadenas de caracteres a mostrar en el LCD y UART
volatile uint16_t datoADC = 0; // Declarar como volatile para uso en ISR

////////////////////////////////////////////////////
// Prototipos de funciones
////////////////////////////////////////////////////
void display_menu(void);
void process_command(char command);
void setup(void);

////////////////////////////////////////////////////
// Sub-Rutinas
////////////////////////////////////////////////////
void setup(void){
    cli();  // Desactivar interrupciones
    DDRC = 0;  // Puerto C como entrada
    initADC(); // Iniciar ADC
    UART_Init(BAUD);  // Inicializar UART
    //I2C_Config_SLAVE(0x02); // Inicializar I2C como esclavo con dirección 0x02
    display_menu(); // Mostrar el menú inicial en la consola
    sei(); // Activar interrupciones
}

////////////////////////////////////////////////////
// Programa Principal
////////////////////////////////////////////////////
int main(void) {
    setup();

    while (1) {
        // Leer ADC y actualizar datoADC en la ISR
        ADCSRA |= (1 << ADSC);  // Iniciar conversión ADC
        _delay_ms(20);   // Retardo para evitar malos procesamientos del Atmega

        // Leer y procesar los comandos de la UART
        if (UCSR0A & (1 << RXC0)) {  // Verificar si hay datos disponibles en el buffer de recepción
            char received_char = UART_Receive();
            process_command(received_char);
        }

        _delay_ms(500);  // Espera medio segundo antes de la siguiente actualización
    }
}

////////////////////////////////////////////////////
// Vectores de interrupciones
////////////////////////////////////////////////////
ISR(ADC_vect) {
    datoADC = ADC;  // Lectura de potenciómetro (16 bits: ADCL y ADCH)
    ADCSRA |= (1 << ADIF); // Borrar la bandera de interrupción
}

////////////////////////////////////////////////////
// Funciones auxiliares
////////////////////////////////////////////////////
void display_menu(void) {
    UART_TransmitString("\r\n***** Menu *****\r\n");
    UART_TransmitString("1. Mostrar valores de ADC\r\n");
    UART_TransmitString("*****************\r\n");
    UART_TransmitString("Seleccione una opción: ");
}

void process_command(char command) {
    switch (command) {
        case '1':
            snprintf(buffer, sizeof(buffer), "Valores ADC: %u\r\n", datoADC);
            UART_TransmitString(buffer);
            break;
        default:
            UART_TransmitString("Opción inválida. Por favor, seleccione una opción válida.\r\n");
            break;
    }
    display_menu();
}
