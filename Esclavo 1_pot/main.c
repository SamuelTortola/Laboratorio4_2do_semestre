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


#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>


#include "ADC/ADC.h"   //Incluir libreria de ADC
#include "I2C/I2C.h"   //Incluir libreria de I2C



uint8_t datoADC = 0;



void setup(void);
void setup(void){
	
	cli();  //Apagar interrupciones
	DDRC =0;  //Puerto C como entrada
	initADC(); //Iniciar ADC
	
	sei(); //Activar interrupciones
}





int main(void)
{
    setup();
	
    while (1) 
    {
		ADCSRA |=(1<<ADSC);  //Leer ADC
		_delay_ms(20);   //Retardo para evitar malos procesamientos del Atmega328P
    }
}




ISR(ADC_vect){
	datoADC = ADCH;   //Lectura de potenciómetro
	ADCSRA |= (1<<ADIF); //Se borra la bandera de interrupción
	
}

