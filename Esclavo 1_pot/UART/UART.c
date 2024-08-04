#include "UART.h"
#define F_CPU 16000000
void UART_init(uint32_t baudrate) {
	uint16_t ubrr = F_CPU / 16 / baudrate - 1;
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Habilitar transmisi�n y recepci�n
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configurar el formato de trama: 8 bits de datos, 1 bit de parada
}

void UART_sendChar(char c) {
	while (!(UCSR0A & (1 << UDRE0)));  // Esperar hasta que el buffer est� vac�o
	UDR0 = c;  // Enviar el car�cter
}

void UART_sendString(const char* str) {
	while (*str) {
		UART_sendChar(*str);
		str++;
	}
}
