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
 * @file config.h
 * @brief Configuration macros for AVR software UART.
 * @author notweerdmonk
 */

#ifndef CONFIG_H
#define CONFIG_H

#define c_UART_PARITY_NONE  -1
#define c_UART_PARITY_EVEN  1
#define c_UART_PARITY_ODD   2
#define c_UART_PARITY_MARK  3
#define c_UART_PARITY_SPACE 4

#define c_UART_BAUD_RATE    4800

#define c_UART_CHAR_SIZE    8

#define c_UART_PARITY_TYPE  c_UART_PARITY_NONE

#define c_UART_N_STOP_BITS  1

#define c_UART_BUF_SIZE     16

#endif /* CONFIG_H */
