/**
 * @file uart.h
 * @brief Function prototypes for software UART.
 *
 * This file contains prototypes for software UART. Include
 * it when using the library archive.
 *
 * @author notweerdmonk
 */

#ifndef UART_H
#define UART_H

#include <avr/interrupt.h>

/**
 * @brief Initializes sofware UART.
 * @param baud_rate The baud rate.
 * @return void
 */
void setup_uart(unsigned long baud_rate);

/**
 * @brief Sends a single character while blocking if outbound buffer is full.
 * @param c The character to send.
 * @return Void
 */
void send(char c);

/**
 * @brief Sends a single character but does not block if outbound buffer
 *        is full.
 * @param c The character to send.
 * @return Zero on failure, non-zero value on success.
 */
char send2(char c);

/**
 * @brief Sends a string of characters.
 * @param str A pointer to the string.
 * @return Void
 */
void send_str(const char *str);

/**
 * @brief Receives a single character while blocking if inbound buffer is full.
 * @return The received character.
 */
char recv();

/**
 * @brief Receives a single character but does not block if inbound buffer
 *        is full.
 * @return The received character.
 */
char recv2();

/**
 * @brief Receives a string of characters.
 *
 * The function will append a null character at the end of the string.
 *
 * @param str A pointer to an array memory location sufficiently large to
 *        accomodate the string including null character.
 * @param n The length of the string to receive, excluding null character.
 * @return Void
 */
void recv_str(const char *str, int n);

#endif /* UART_H */
