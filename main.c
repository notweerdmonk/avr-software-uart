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
 * @file main.c
 * @brief Driver program which demonstrates the use of AVR software UART.
 * @author notweerdmonk
 */

#include <uart.h>
#include <defines.h>

#define TRIGGER_DDR  DDRB
#define TRIGGER_PORT PORTB
#define TRIGGER_PIN  PB4
#define EMIT_TRIGGER

#if defined SIMULATION
#define STRINGIFY(s) #s
#define MCU_NAME(m) STRINGIFY(m)
#define VCD_FILE(m) STRINGIFY(./simulation/)STRINGIFY(m)STRINGIFY(_uart_trace.vcd)

#include </usr/include/simavr/avr/avr_mcu_section.h>
AVR_MCU(16000000, MCU_NAME(DEVICE_NAME));
AVR_MCU_VCD_FILE(VCD_FILE(DEVICE_NAME), 1000);

const struct avr_mmcu_vcd_trace_t _trace[] _MMCU_ = {
  { AVR_MCU_VCD_SYMBOL("TxD"), .mask = (1 << TX_PIN), .what = (void*)&TX_PORT, },
  { AVR_MCU_VCD_SYMBOL("RxD"), .mask = (1 << RX_PIN), .what = (void*)&RX_PORT, },
  { AVR_MCU_VCD_SYMBOL("Trigger"), .mask = (1 << TRIGGER_PIN), .what = (void*)&TRIGGER_PORT, },
};
#endif

char *pattern1 = "UUUU";
char *pattern2 = "AAAAAAAAAAAAAAAA";
char *pattern3 = "aaaaaaaaaaaaaaaa";
char *pattern4 = "abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main() {
  char *teststring = pattern4;

  setup_uart(0);
  sei();

#if defined EMIT_TRIGGER
  /* wait a while */
  asm volatile ("ldi r24, 255 \n\t"
                "loop1:       \n\t"
                "ldi r25, 50  \n\t"
                "loop2:       \n\t"
                "dec r25      \n\t"
                "brne loop2   \n\t"
                "dec r24      \n\t"
                "brne loop1   \n\t"
                :
                :
               );
  TRIGGER_DDR |= (1 << TRIGGER_PIN);
  TRIGGER_PORT |= (1 << TRIGGER_PIN);
#endif

  send_str(teststring);
  send('\n'); send('\r');

  /* loopback */
  for(;;send(recv()));

  return 0;
}
