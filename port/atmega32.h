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
 * @file atmega32.h
 * @brief Portablity macros for ATmega32
 * @author notweerdmonk
 */

#ifndef PORT_ATMEGA32_H
#define PORT_ATMEGA32_H

#define PORT_TX_PIN  PD3
#define PORT_TX_DDR  DDRD
#define PORT_TX_PORT PORTD
#define PORT_TX_INP  PIND

#define PORT_RX_PIN  PD2
#define PORT_RX_DDR  DDRD
#define PORT_RX_PORT PORTD
#define PORT_RX_INP  PIND

#define PORT_RX_INT_EN INT0

#define PORT_BAUD_TIMER_COUNT TCNT1
#define PORT_BAUD_TIMER_TOP   OCR1B

#define port_rx_capture_isr INT0_vect

#define port_rx_isr TIMER1_COMPB_vect

#define port_tx_isr TIMER1_COMPA_vect

#define port_enable_rx_capture() \
  GICR  |= (1<<RX_INT_EN)

#define port_disable_rx_capture() \
  GICR  &= ~(1<<RX_INT_EN)

#define port_setup_rx_capture() \
  do { \
    port_enable_rx_capture(); \
  } while(0)

#define port_clear_rx_capture_isr() \
  TIFR |= (1<<OCF1B);

#define port_enable_rx_baud_timer() \
  TIMSK |= (1<<OCIE1B)

#define port_disable_rx_baud_timer() \
  TIMSK &= ~(1<<OCIE1B)

#define port_setup_baud_timer(fcpu, baudrate) \
  do { \
    TIMSK |= (1<<OCIE1A); \
    TCCR1A = 0; \
    TCCR1B = (1<<WGM12) | (1<<CS10); \
    TCNT1  = 0; \
    OCR1A  = (unsigned int)(fcpu * 1/baudrate + 0.5); \
  } while(0)

#endif /* PORT_ATMEGA32_H */
