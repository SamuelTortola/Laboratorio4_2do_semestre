//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094, Alan Gomez - 22115
// Proyecto: Laboratorio 3
// Hardware: Atmega238p
// Creado: 26/07/2024
//Última modificación: 29/07/2024
//******************************************************************************
  //CODIGO DEL ESCLAVO QUE TIENE EL POTENCIÓMETRO
  
/////////////////////////////////////////////
//Librerias Primarias
/////////////////////////////////////////////

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////
//Librerias Secundarias
/////////////////////////////////////////////
#include "ADC/ADC.h"   //Incluir libreria de ADC
#include "I2C/I2C.h"   //Incluir libreria de I2C

/////////////////////////////////////////////
//Variables
/////////////////////////////////////////////

uint8_t datoADC = 0;
uint8_t dir = 0x02; //Direccion del esclavo
/////////////////////////////////////////////
//Sub-Rutinas
/////////////////////////////////////////////

void setup(void);
void setup(void){
	
	cli();  //Apagar interrupciones
	DDRC =0;  //Puerto C como entrada
	initADC(); //Iniciar ADC
	
	dir <<= 1;  //Ubica la direcci?n y
	dir |= 0x01; //habilita para reconocer las llamadas generales de I2C
	TWAR = dir;
	
	TWCR=(1 << TWEA) | (1<< TWEN) | (1 << TWIE);
	sei(); //Activar interrupciones
}
void Interfaz(void);
/////////////////////////////////////////////
//Codigo Principal
/////////////////////////////////////////////
int main(void)
{
    setup();
	
    while (1) 
    {
		ADCSRA |=(1<<ADSC);  //Leer ADC
		_delay_ms(20);   //Retardo para evitar malos procesamientos del Atmega328P
    }
}

/////////////////////////////////////////////
//Vectores de interrupciones
/////////////////////////////////////////////
ISR(ADC_vect){
	datoADC = ADCH;   //Lectura de potenciómetro
	ADCSRA |= (1<<ADIF); //Se borra la bandera de interrupción
}

