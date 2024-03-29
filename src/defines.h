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
 * @file defines.h
 * @brief Definitions for macros used in AVR software UART.
 * @author notweerdmonk
 */

#ifndef DEFINES_H

#include <config.h>
#include <port.h>

#ifndef DEFAULT_BAUD_RATE
#define DEFAULT_BAUD_RATE c_UART_BAUD_RATE
#endif

#ifndef CHAR_SIZE
#define CHAR_SIZE c_UART_CHAR_SIZE
#endif

#ifdef NUM_START_BITS
#undef NUM_START_BITS
#endif
#define NUM_START_BITS 1

#ifndef NUM_STOP_BITS
#define NUM_STOP_BITS c_UART_N_STOP_BITS
#endif

#ifndef PARITY_TYPE
  #if c_UART_PARITY_TYPE != c_UART_PARITY_NONE && \
      c_UART_PARITY_TYPE != c_UART_PARITY_EVEN && \
      c_UART_PARITY_TYPE != c_UART_PARITY_ODD  && \
      c_UART_PARITY_TYPE != c_UART_PARITY_MARK && \
      c_UART_PARITY_TYPE != c_UART_PARITY_SPACE
    #error macro c_UART_PARITY_TYPE not defined
  #else
    #define PARITY_TYPE c_UART_PARITY_TYPE
  #endif
#endif /* PARITY_TYPE */

#ifndef BUF_SIZE
#define BUF_SIZE c_UART_BUF_SIZE
#endif

#ifndef TX_PIN
#define TX_PIN PORT_TX_PIN
#endif

#ifndef TX_DDR
#define TX_DDR PORT_TX_DDR
#endif

#ifndef TX_PORT
#define TX_PORT PORT_TX_PORT
#endif

#ifndef TX_INP
#define TX_INP PORT_TX_INP
#endif

#ifndef RX_PIN
#define RX_PIN PORT_RX_PIN
#endif

#ifndef RX_DDR
#define RX_DDR PORT_RX_DDR
#endif

#ifndef RX_PORT
#define RX_PORT PORT_RX_PORT
#endif

#ifndef RX_INP
#define RX_INP PORT_RX_INP
#endif

#ifndef RX_INT_EN
#define RX_INT_EN PORT_RX_INT_EN
#endif

#ifndef RX_INT_PIN
#define RX_INT_PIN PORT_RX_INT_PIN
#endif

#ifndef RX_INT_MSK
#define RX_INT_MSK PORT_RX_INT_MSK
#endif

#endif /* DEFINES_H */
