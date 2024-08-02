#include "I2C.h"

 void I2C_Init(){

	/////// Setting frequency ///////
	/*
		F = 	CPUclk/(16 + (2*TWBR*Prescaler))
		F =		20000/(16 + 2*42*1)
		F =		200kHz
	*/

	//Prescaler
	TWSR &=~ (1<<TWPS0);
	TWSR &=~ (1<<TWPS1);

	//factor divisor
	TWBR = 17;

	/////// I2C POWER ON ///////
	PRR	 &=~ (1<<PRI2C);

 }

 bool I2C_startCond(){
	TWCR = ((1<<I2CNT) | (1<<TWSTA) | (1<<TWEN));

	while(!(TWCR & (1<<I2CNT)));

	if ((TWSR & 0xF8) == I2C_START)
		return false;

	return true;
 }

 bool I2C_restrtCond(){
   TWCR = ((1<<I2CNT) | (1<<TWSTA) | (1<<TWEN));

   while(!(TWCR & (1<<I2CNT)));

   if ((TWSR & 0xF8) == I2C_RESTART)
	  return false;

   return true;
 }

 void I2C_stopCond(){
	TWCR |= ((1<<I2CNT) | (1<<TWSTO) | (1<<TWEN));
 }

 bool I2C_sendAdrr(uint8_t adrr, uint8_t action){
	
	uint8_t cmp = 0;
	adrr = (adrr << 1 );

	if (action == I2C_W){
		adrr &=~ 1;
		cmp = I2C_WT_SLA_ACK;
	}
	else{
		adrr |= 1;
		cmp = I2C_RD_SLA_ACK;
	}

	TWDR = adrr;
	TWCR = ((1<<I2CNT) | (1<<TWEN));

	while(!(TWCR & (1<<I2CNT)));

	if ((TWSR & 0xF8) == cmp)
		return false;
	 
	return true;
 }

 bool I2C_write(uint8_t data2write){
	
	bool ret = true;
	
	TWDR = data2write;
	TWCR = ((1<<I2CNT) | (1<<TWEN));
	while(!(TWCR & (1<<I2CNT)));
	
	if ((TWSR & 0xF8) == I2C_MT_DATA_ACK)
		ret = false;
	
	return ret;
 }

 uint8_t I2C_read(uint8_t ACK_NACK){
	
	TWCR = ((1 << I2CNT) | (1 << TWEN) | (ACK_NACK << TWEA));

	while(!(TWCR & (1<<I2CNT)));
	return TWDR;
 }