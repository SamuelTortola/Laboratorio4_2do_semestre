
#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <stdbool.h>


#define	I2C_W			0x00
#define	I2C_R			0x01
#define I2C_ACK			0x01
#define I2C_NACK		0x00

#define I2C_START		0x08
#define I2C_RESTART		0x10
#define I2C_WT_SLA_ACK	0x18
#define I2C_MT_DATA_ACK	0x28
#define I2C_RD_SLA_ACK	0x40

void I2C_Init();
bool I2C_startCond();
bool I2C_restrtCond();
void I2C_stopCond();
bool I2C_write(uint8_t);
uint8_t I2C_read(uint8_t);
bool I2C_sendAdrr(uint8_t, uint8_t);



#endif 