
#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <stdbool.h>
#define F_CPU 16000000

 void I2C_Config_MASTER(uint8_t Prescaler, unsigned long SCL_Clock);
 void I2C_Config_SLAVE(uint8_t address);
 void I2C_esclavo(uint8_t dato);
 uint8_t I2C_inicio();
 uint8_t I2C_EscByte(uint8_t dato);
 void I2C_STOP();
 uint8_t I2C_READ(uint8_t *dato, uint8_t ack);

#endif 