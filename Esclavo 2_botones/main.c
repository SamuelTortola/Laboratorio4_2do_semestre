//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electr�nica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega238p
// Creado: 26/07/2024
//�ltima modificaci�n: 2/07/2024
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
uint8_t dir = 0x03; //Direcci�n del esclavo


void setup(void);
void setup(void){
	DDRB = 0b00000000;   //PB0, PB1 como entrada de pulsadores
	DDRD = 0b00001111;  //Salida hacia LEDs del contador de 4 bits

	PORTB = 0b00000011;		//pull up encendido en PB0 y PB1 
	PORTD = 0b00000000;   //Iniciamos los leds apagados
	
	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	PCMSK0 |= (1 << 0)|(1 << 1); //PCINT0, PCINT1
	PCICR |= (1 << 0); //Mascara de interrupci�n
	
	dir <<= 1;  //Ubica la direcci�n y 
	dir |= 0x01; //habilita para reconocer las llamadas generales de I2C
	TWAR = dir;
	
	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //Habilita el BUS, con reconocimiento e interrupci�n
	
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
	
	
	if(((PINB) & (1<<0)) == 0){   //Condicional que compara si se presion� el pulsador 1
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
	
	if(((PINB) & (1<<1)) == 0){   //Condicional que compara si se presion� el pulsador  de start
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

ISR(TWI_vect){
	uint8_t dato, estado;
	
	estado = TWSR & 0xFC;  //Lee el estado de la interfaz
	
	switch(estado){
		case 0x60:
		case 0x70:              //Direccionado con su direccion de esclavo
			TWCR |= (1 << TWINT); //
			break;
			
		case 0x80:
		case 0x90:
			dato = TWDR;  //Recibi� el dato, llamada general
			PORTD = dato; 
			TWCR |= 1 << TWINT; //Borra la bandera TWINT
			break;
		default:    //Libera el BUS de cualquier errror
			TWCR |= (1 << TWINT) | (1 << TWSTO);
			
	}
	
}

