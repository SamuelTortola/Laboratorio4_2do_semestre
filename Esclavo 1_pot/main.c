//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega328p
// Creado: 26/07/2024
// Última modificación: 29/07/2024
//******************************************************************************
// CODIGO DEL ESCLAVO QUE TIENE EL POTENCIÓMETRO

/////////////////////////////////////////////
// Librerías Primarias
/////////////////////////////////////////////

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////
// Librerías Secundarias
/////////////////////////////////////////////
#include "ADC/ADC.h"   // Incluir librería de ADC
#include "I2C/I2C.h"   // Incluir librería de I2C
#include "UART/UART.h" // Incluir librería de UART

/////////////////////////////////////////////
// Variables
/////////////////////////////////////////////
volatile int datoADC = 0, dato;
char buffer[16]; // Buffer para almacenar la cadena de caracteres

/////////////////////////////////////////////
// Sub-Rutinas
/////////////////////////////////////////////

void setup(void);
void setup(void){
    cli();  // Apagar interrupciones
    DDRC = 0;  // Puerto C como entrada
    initADC(); // Iniciar ADC
    I2C_Config_SLAVE(0x02); // Configurar como esclavo I2C
    UART_init(9600); // Configurar UART a 9600 baudios
    sei(); // Activar interrupciones
}

/////////////////////////////////////////////
// Código Principal
/////////////////////////////////////////////
int main(void)
{
    setup();
    
    while (1) 
    {
        ADCSRA |= (1<<ADSC);  // Iniciar conversión ADC

        // Enviar el valor del ADC a través de UART
        snprintf(buffer, sizeof(buffer), "ADC: %d\r\n", datoADC);
        UART_sendString(buffer);
		_delay_ms(20);   // Retardo para evitar malos procesamientos del Atmega328P
    }
}

/////////////////////////////////////////////
// Vectores de interrupciones
/////////////////////////////////////////////
ISR(ADC_vect){
    datoADC = ADCH; // Lectura del valor completo del ADC (10 bits)
    ADCSRA |= (1<<ADIF); // Borrar la bandera de interrupción
}

ISR(TWI_vect){ // Vector de interrupción de I2C
    uint8_t estado;
    
    estado = TWSR & 0xFC;  // Leer el estado de la interfaz
    
    switch(estado){
        case 0x60:
        case 0x70: // Direccionado con su dirección de esclavo
            TWCR |= (1 << TWINT);
            break;
        
        case 0x80:
        case 0x90:
            dato = TWDR;  // Recibió el dato, llamada general
            TWCR |= (1 << TWINT); // Borrar la bandera TWINT
            break;
        
        case 0xA8: // SLA+R recibido, maestro solicita lectura
        case 0xB8: // Dato transmitido y ACK recibido
            TWDR = datoADC; // Cargar el dato en el registro de datos
            TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA); // Listo para la próxima operación
            break;
        
        case 0xC0: // Dato transmitido y NACK recibido
        case 0xC8: // Último dato transmitido y ACK recibido
            TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Listo para la próxima operación
            break;
        
        default: // Liberar el BUS de cualquier error
            TWCR |= (1 << TWINT) | (1 << TWSTO);
            break;
    }
}
