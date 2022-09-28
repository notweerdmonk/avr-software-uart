/*
  MIT License
  
  Copyright (c) 2020 - 2022 notweerdmonk
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * @file uart.h
 * @brief Function prototypes for AVR software UART.
 *
 * This file contains prototypes for AVR software UART. Include
 * it when using the library archive.
 *
 * @author notweerdmonk
 */

#ifndef UART_H
#define UART_H

#include <avr/interrupt.h>

/**
 * @brief Initializes software UART.
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
