//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega238p
// Creado: 26/07/2024
//Última modificación: 29/07/2024
//******************************************************************************
  //CODIGO DEL ESCLAVO QUE TIENE LOS BOTONES


#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "I2C/I2C.h"   //Incluir libreria de I2C

int contador = 0;

void setup(void);
void setup(void){
	DDRB = 0b00000000;   //PB0, PB1 como entrada de pulsadores
	DDRD = 0b00001111;  //Salida hacia LEDs del contador de 4 bits

	PORTB = 0b00000011;		//pull up encendido en PB0 y PB1 
	PORTD = 0b00000000;   //Iniciamos los leds apagados
	
	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	PCMSK0 |= (1 << 0)|(1 << 1); //PCINT0, PCINT1
	PCICR |= (1 << 0); //Mascara de interrupción
	
	sei(); //Activar interrupciones
}

int main(void)
{
    setup();
    while (1) 
    {
		
		_delay_ms(10);
		
    }
}


ISR(PCINT0_vect){
	
	
	if(((PINB) & (1<<0)) == 0){   //Condicional que compara si se presionó el pulsador 1
		_delay_ms(50);  //antirrebote
		contador ++;
		
		if (contador >= 15)
		{
			contador = 15;
		}
		
		PORTD = contador;
		
		  
		while ((PINB & (1 << PINB0)) == 0)   //While para evitar sumas indebidas
		{
			_delay_ms(10);
		}
		
	}
	
	if(((PINB) & (1<<1)) == 0){   //Condicional que compara si se presionó el pulsador  de start
		_delay_ms(50);  //antirrebote
		
		contador --;
		
			if (contador <= 0)
			{
				contador = 0;
			}
			PORTD = contador;
		
		while ((PINB & (1 << PINB1)) == 0)   //While para evitar sumas indebidas
		{
			_delay_ms(10);
		}
		
	}
	
	
	
}

