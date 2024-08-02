//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega238p
// Creado: 26/07/2024
//Última modificación: 2/08/2024
//******************************************************************************
  //CODIGO DEL MAESTRO 


#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/twi.h>

#include "I2C/I2C.h"   //Incluir libreria de I2C
#include "LCD/LCD.h"     //Incluir libreria de LCD


# define esclavo1 0x03  //Dirección del esclavo 1
# define esclavo2 0x02   //Dirección del esclavo 2

uint8_t dato1; //Variable para almacenar el dato que envia el esclavo
uint8_t dato_leido; //Variable que almacena el valor en si


void setup(void);
void setup(void){
	cli();  //Apagar interrupciones
	DDRD = 0xFF;  //Puerto D como salida
	DDRB = 0xFF;  //Puerto B como salida
	
	
	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	Lcd_Init8bits();   //Iniciar pantalla LCD
	Lcd_Clear();
	
	I2C_Config_MASTER(4, 200);
	
	sei(); //Activar interrupciones
}


int main(void)
{
   setup();
    while (1) 
    {
		  _delay_ms(1000);
		  Lcd_Set_Cursor(0,3);
		  Lcd_Write_String("S1:");
		  Lcd_Set_Cursor(0,10);
		  Lcd_Write_String("S2:");
		
		//  I2C_esclavo(1, esclavo1);
		  
		  
		  dato_leido = I2C_leer_dato(esclavo1, &dato1);
		 
		  if (dato_leido == 2) {
			   Lcd_Set_Cursor(1,10);
			   Lcd_Write_String("2");
			  } 
			  
		else{
			 Lcd_Set_Cursor(1,10);
			 Lcd_Write_String("1");
		}
			  
		
		
    }
}

