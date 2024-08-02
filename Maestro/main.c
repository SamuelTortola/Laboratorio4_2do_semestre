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

uint8_t dato1, dato2; //Variable para almacenar el dato que envia el esclavo
uint8_t dato_leido_1, dato_leido_2; //Variable que almacena el valor en si
char buffer[64];  // Buffer para las cadenas de caracteres a mostrar en el LCD


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
		  _delay_ms(10);
		  Lcd_Set_Cursor(0,3);
		  Lcd_Write_String("S1:");
		  Lcd_Set_Cursor(0,10);
		  Lcd_Write_String("S2:");
		
		//  I2C_esclavo(1, esclavo1); //Por si se desea enviar un valor a algún esclavo
		  
		  
		  dato_leido_1 = I2C_leer_dato(esclavo1, &dato1);
		  dato_leido_2 = I2C_leer_dato(esclavo2, &dato2);
		 
		 Lcd_Set_Cursor(1,3);
		 snprintf(buffer, 16, "%d", dato_leido_2);
		 Lcd_Write_String(buffer);
		
	
		 
		 switch(dato_leido_1){
			 case 0:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("00");
				 break;
				 
			 case 1:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("01");
				 break;
				 
			 case 2:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("02");
				 break;
			 
			 case 3:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("03");
				 break;
				 
			case 4:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("04");
				 break;
				 
			 case 5:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("05");
				 break;
				 
			 case 6:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("06");
				 break;
			 
			 case 7:
				 Lcd_Set_Cursor(1,10);
				 Lcd_Write_String("07");
				 break;
				 
			case 8:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("08");
				break;
			
			case 9:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("09");
				break;
			
			case 10:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("10");
				break;
			
			case 11:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("11");
				break;
			
			case 12:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("12");
				break;
			
			case 13:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("13");
				break;
			
			case 14:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("14");
				break;
			
			case 15:
				Lcd_Set_Cursor(1,10);
				Lcd_Write_String("15");
				break;
			default:
				
				break;
				
		 }
		 
		 
		
			  
		
		
    }
}

