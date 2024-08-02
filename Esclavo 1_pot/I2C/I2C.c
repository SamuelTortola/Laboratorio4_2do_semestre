#include "I2C.h"


uint8_t esclavo, dato, aux;




//********Datos a saber*****************
    //SLA = dirección del esclavo
	
	
	
	/////// Setting frequency ///////
	/*
		F = 	CPUclk/(16 + (2*TWBR*Prescaler))
		F =		20000/(16 + 2*42*1)
		F =		200kHz
	*/
	
	
	
	
void I2C_Config_MASTER(uint8_t Prescaler, unsigned long SCL_Clock){
	DDRC &= ~((1<<DDC4) | (1<<DDC5));
	
	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler);
	
	
	switch(Prescaler){
		case 1:
			TWSR &= ~((1<<TWPS1) | (1<<TWPS0));
			break;
		case 4:
			TWSR &= ~(1<<TWPS1);
			TWSR |= (1<<TWPS0);
			break;
		case 16:
			TWSR &= ~(1<<TWPS0);
			TWSR |= (1<<TWPS1);
			break;
			
		case 64:
			TWSR |= ~(1<<TWPS1) | (1<<TWPS0);
			break;
		default:
			TWSR &= ~((1<<TWPS1) | (1<<TWPS0));
			Prescaler = 1;
			break;
			
	}
	
	TWCR = 1 << TWEN;   //Habilita la interfaz
}

void I2C_Config_SLAVE(uint8_t address){
	DDRC &= ~((1<<DDC4) | (1<<DDC5));
	
	address <<= 1;  //Ubica la dirección y
	address |= 0x01; //habilita para reconocer las llamadas generales de I2C
	TWAR = address;
	
	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //Habilita el BUS, con reconocimiento e interrupción
}


uint8_t I2C_inicio(){
	uint8_t edo;  //Variable que indica que no se consiguió el bus 
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA); //Condición de inicio
	while (!(TWCR & (1 << TWINT)));  //Espera la bandera TWINT
	edo = TWSR & 0xF8; //Obtiene el estado, 1 condicion de inicio, o 2 inicio repetido
	
	if (edo == 0x08 || edo == 0x10) 
	{
		return 0x01;
	}
	
	return edo;  
	
}



//I2C_EscByte: Escribe un dato o una SLA + W/R por el bus
//Recibe: El dato de 8 bits a enviar
//Regresa: 0x01 envio sin problemas, edo, si el dato no se envio como se esperaba

uint8_t I2C_EscByte(uint8_t dato){
	uint8_t edo;
	
	TWDR = dato;      //Carga el dato   
	TWCR = (1 << TWEN) | (1 << TWINT);   //Inicia el envio
	
	while (!(TWCR & (1 << TWINT))); //espera la bandera TWINT
	edo = TWSR & 0xF8; //Obtiene el estado
	
	    //Hay 3 posibilidades de éxito:
	if (edo == 0x18 || edo == 0x28 || edo == 0x40)  //Transmitió una SLA+W CON ACK, transimitió una SLA+R con ACK, Transmitió un dato con ACK
	{
		return 0x01;
	}
	return edo;   //So hay algún error
	
}

void I2C_STOP(){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);  //Condición de Paro
	while (TWCR & (1 << TWSTO));  //El bit se limpia por HW
}

uint8_t I2C_READ(uint8_t *dato, uint8_t ack){
	uint8_t edo;
	
	if (ack)
	{
		TWCR |= (1 << TWEA); //Lectura con ACK
	}
	
	else{
		TWCR &= ~(1 << TWEA); //Lectura con nACK
	}
	
	TWCR |= (1 <<TWINT); //Inicia la lectura
	while (!(TWCR & (1 << TWINT))); //Espera la bandera TWINT
	edo = TWSR & 0xF8;   //Obtiene el estado
	
	if (edo == 0x58 || edo == 0x50)  //Dato leido con ACK, dato leido con nACK
	{
		*dato = TWDR;//Ubica el dato leido
		return 0x01;
	}
	
	return edo;    //Si hay algun error
	
}



void I2C_esclavo(uint8_t dato){   //Envia el dato al esclavo
	esclavo = 0x02;  //Dirección del esclavo
	esclavo = esclavo << 1;    //Compone la SLA+W
	
	
	aux = I2C_inicio();   //Condicion de inicio
	if (aux != 0x01)  //Si no se establece la conexión
	{
		TWCR |= (1 << TWINT);  //Borrar la bandera
		return;   //No continua
	}
	
	aux = I2C_EscByte(esclavo);     //Direcciona con la SLA+W
	if (aux != 0x01)   //Si hay algun error de comunicación
	{
		I2C_STOP();  //No continua, termina la comunicación y la cierra
		return;   
	}
	
	I2C_EscByte(dato);   //Envia el dato al esclavo
	I2C_STOP();  //No continua, termina la comunicación y la cierra
	
	
}