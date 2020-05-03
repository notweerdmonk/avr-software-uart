#ifndef UART_H
#define UART_H

#include <avr/interrupt.h>

void setup_uart(unsigned long baud_rate);

/* blocking */
void send(char c);

/* non-blocking */
char send2(char c);

void send_str(const char *str);

/* blocking */
char recv();

/* non-blocking */
char recv2();

void recv_str();

#endif /* UART_H */
