//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electr�nica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega328p
// Creado: 26/07/2024
// �ltima modificaci�n: 2/08/2024
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

#include "I2C/I2C.h"  // Incluir librer�a de I2C
#include "LCD/LCD.h"  // Incluir librer�a de LCD

#define ESCLAVO1 0x03  // Direcci�n del esclavo 1
#define ESCLAVO2 0x02  // Direcci�n del esclavo 2

int dato1;
int dato2;  // Variables para almacenar los datos que env�an los esclavos
char buffer[16];  // Buffer para las cadenas de caracteres a mostrar en el LCD

void setup(void) {
    cli();  // Apagar interrupciones
    DDRD = 0xFF;  // Puerto D como salida
    DDRB = 0xFF;  // Puerto B como salida
    
    UCSR0B = 0;  // Usar los pines TX y RX como digitales
    
    Lcd_Init8bits();  // Iniciar pantalla LCD
    Lcd_Clear();
    
    I2C_Config_MASTER(4, 200000);  // Configurar I2C como maestro, 4 de divisi�n de prescaler, a 200kHz de comunicaci�n
    
    sei();  // Activar interrupciones
}

int main(void) {
    setup();
    while (1) {
        _delay_ms(10);   //Peque�o retardo 
        
        Lcd_Set_Cursor(0, 3);     //Mostrar el men� en la LCD
        Lcd_Write_String("S1:");
        Lcd_Set_Cursor(0, 10);
        Lcd_Write_String("S2:");
        
        dato1 = I2C_leer_dato(ESCLAVO1, &dato1);   //Leer el dato del esclavo 1
        dato2 = I2C_leer_dato(ESCLAVO2, &dato2);    //Leer el dato del esclavo 2
        
        Lcd_Set_Cursor(1, 3);
        snprintf(buffer, sizeof(buffer), "%03d", dato2);    //Convertir el dato num�rico a char para mostrar en la LCD
	
		
        Lcd_Write_String(buffer);
        
        Lcd_Set_Cursor(1, 10);
        snprintf(buffer, sizeof(buffer), "%02d", dato1);  //Convertir el dato num�rico a char para mostrar en la LCD
        Lcd_Write_String(buffer);
    }
}
