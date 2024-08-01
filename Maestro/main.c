//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega238p
// Creado: 26/07/2024
//Última modificación: 29/07/2024
//******************************************************************************
  //CODIGO DEL MAESTRO 


#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "I2C/I2C.h"   //Incluir libreria de I2C
#include "LCD/LCD.h"     //Incluir libreria de LCD



void setup(void);
void setup(void){
	cli();  //Apagar interrupciones
	DDRD = 0xFF;  //Puerto D como salida
	DDRB = 0xFF;  //Puerto B como salida
	
	

	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	Lcd_Init8bits();   //Iniciar pantalla LCD
	Lcd_Clear();
	
	sei(); //Activar interrupciones
}


int main(void)
{
   setup();
    while (1) 
    {
		
		
		
    }
}

