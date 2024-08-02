//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega328p
// Creado: 26/07/2024
// Última modificación: 2/08/2024
//******************************************************************************

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/twi.h>

#include "I2C/I2C.h"  // Incluir librería de I2C
#include "LCD/LCD.h"  // Incluir librería de LCD

#define ESCLAVO1 0x03  // Dirección del esclavo 1
#define ESCLAVO2 0x02  // Dirección del esclavo 2

uint8_t dato1, dato2;  // Variables para almacenar los datos que envían los esclavos
char buffer[8];  // Buffer para las cadenas de caracteres a mostrar en el LCD

void setup(void) {
    cli();  // Apagar interrupciones
    DDRD = 0xFF;  // Puerto D como salida
    DDRB = 0xFF;  // Puerto B como salida
    
    UCSR0B = 0;  // Usar los pines TX y RX como digitales
    
    Lcd_Init8bits();  // Iniciar pantalla LCD
    Lcd_Clear();
    
    I2C_Config_MASTER(4, 200);  // Configurar I2C como maestro
    
    sei();  // Activar interrupciones
}

int main(void) {
    setup();
    while (1) {
        _delay_ms(10);
        
        Lcd_Set_Cursor(0, 3);
        Lcd_Write_String("S1:");
        Lcd_Set_Cursor(0, 10);
        Lcd_Write_String("S2:");
        
        dato1 = I2C_leer_dato(ESCLAVO1, &dato1);
        dato2 = I2C_leer_dato(ESCLAVO2, &dato2);
        
        Lcd_Set_Cursor(1, 3);
        snprintf(buffer, 8, "%d", dato2);
        Lcd_Write_String(buffer);
        
        Lcd_Set_Cursor(1, 10);
        snprintf(buffer, 8, "%02d", dato1);
        Lcd_Write_String(buffer);
    }
}
