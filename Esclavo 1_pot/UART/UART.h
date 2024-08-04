#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdint.h>

void UART_init(uint32_t baudrate);
void UART_sendChar(char c);
void UART_sendString(const char* str);

#endif // UART_H
