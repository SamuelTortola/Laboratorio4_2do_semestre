
#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <stdbool.h>

 void I2C_Config(uint8_t Prescaler);
 void I2C_esclavo(uint8_t dato);
 uint8_t I2C_inicio();
 uint8_t I2C_EscByte(uint8_t dato);
 void I2C_STOP();
 uint8_t I2C_READ(uint8_t *dato, uint8_t ack);

#endif 