#include <config.h>
#include <uart.h>
#include <port.h>
#include <defines.h>

#define PARITY_BIT \
  PARITY_TYPE > c_UART_PARITY_NONE

#define EVAL_PARITY \
  PARITY_BIT && PARITY_TYPE < c_UART_PARITY_MARK

#if PARITY_BIT
#define NUM_PARITY_BIT 1
#else
#define NUM_PARITY_BIT 0
#endif

#ifndef FRAME_SIZE
#define FRAME_SIZE NUM_START_BITS + CHAR_SIZE + NUM_PARITY_BIT + NUM_STOP_BITS
#endif

#define WRAP(x, y) if (++(x) == (y)) x = 0

#define rx_pin_state (RX_INP & (1<<RX_PIN))

static struct buffer {
            unsigned char buffer[BUF_SIZE];
  volatile  unsigned char size;
            unsigned char in;
            unsigned char out;
} tx_stream, rx_stream;

static struct shifter {
            unsigned char bits;
#if PARITY_BIT
            unsigned char n      : 4;
            unsigned char parity : 4;
#else
            unsigned char n;
#endif
} tx, rx;

unsigned int baud_timer_offset;

ISR(port_tx_isr) {
  switch(tx.n) {
    case 0: {
      if (tx_stream.size) {
        tx.bits = tx_stream.buffer[tx_stream.out];
        WRAP(tx_stream.out, BUF_SIZE);
        --tx_stream.size;
        tx.n = FRAME_SIZE;
      } else {
        break;
      }
    }
    /* start bit */
    case FRAME_SIZE: {
      TX_PORT &= ~(1<<TX_PIN);
      #if PARITY_BIT
      tx.parity = ((PARITY_TYPE == c_UART_PARITY_ODD ||
        PARITY_TYPE == c_UART_PARITY_MARK)) ? 1 : 0;
      #endif
      break;
    }
    #if PARITY_BIT
    case NUM_STOP_BITS + NUM_PARITY_BIT: {
      #if PARITY_TYPE == c_UART_PARITY_EVEN
      if (tx.parity & 1)
      #else
      if (!(tx.parity & 1))
      #endif
        TX_PORT |= (1<<TX_PIN);
      else
        TX_PORT &= ~(1<<TX_PIN);
      break;
    }
    #endif /* PARITY_BIT */
    /* stop bit */
    #if NUM_STOP_BITS > 1
    case NUM_STOP_BITS - 1:
    #endif
    case NUM_STOP_BITS: {
      TX_PORT |= (1<<TX_PIN);
      break;
    }
    default: {
      #if EVAL_PARITY
      tx.parity ^= (tx.bits & 1);
      #endif
      /* shift out */
      if (tx.bits & 1)
        TX_PORT |= (1<<TX_PIN);
      else
        TX_PORT &= ~(1<<TX_PIN);
      tx.bits >>= 1;
    }
  }
  if (tx.n) tx.n--;
}

/* 
 * a start bit was captured 
 * sample at half bit intervals
 * TIMER1
 *  OCR1B adjusted from TCNT1
 */
ISR(port_rx_capture_isr) {
  /* sample rx pin */
  if (rx_pin_state == 0) {
    unsigned int tcnt = PORT_BAUD_TIMER_COUNT;

    PORT_BAUD_TIMER_TOP = (tcnt > baud_timer_offset) ?
      tcnt - baud_timer_offset : tcnt + baud_timer_offset;

    rx.n = FRAME_SIZE;

    /* sample rx pin again */
    if (rx_pin_state == 0) {
      port_clear_rx_capture_isr();
      port_enable_rx_baud_timer();
      port_disable_rx_capture();
    }
  }
}

/* sample rx */
ISR(port_rx_isr) {
  if (rx.n) {
    switch(rx.n) {
      case FRAME_SIZE: {
        /* start bit */
        /* should not happen for good start bit */
        if (rx_pin_state) {
          port_disable_rx_baud_timer();
          port_enable_rx_capture();
        }
        else {
          rx.bits = 0;
          #if EVAL_PARITY
          rx.parity = (PARITY_TYPE == c_UART_PARITY_ODD) ? 0x80: 0;
          #endif
        }
        break;
      }
      #if PARITY_BIT
      case NUM_STOP_BITS + NUM_PARITY_BIT: {
        #if EVAL_PARITY
        if ((rx_pin_state) != (rx.parity)) {
          /* parity error */
          port_disable_rx_baud_timer();
          port_enable_rx_capture();
        }
        #else
        rx.parity = (rx_pin_state) ? 1 : 0;
        #endif /* PARITY_TYPE < c_UART_PARITY_MARK */
        break;
      }
      #endif /* PARITY_TYPE > c_UART_PARITY_NONE */
      case NUM_STOP_BITS: {
      #if NUM_STOP_BITS == 2
        RX_PORT |= (1<<RX_PIN);
        break;
      }
      case NUM_STOP_BITS - 1: {
      #endif
        /* stop bit */
        if (rx_pin_state) {
          /*
           * use PORT register to save bit value for first stop bit. sample
           * second stop bit from PIN register, then clear PORT pullup and
           * sample stop bit once more.
           *
           * care to check,
           *
           * if the Rx line is pulled low by other end this pin will source
           * current, Tx that side should be able to sink this current.
           *
           * the side effects of writing to PORT for input pins.
           */
          #if NUM_STOP_BITS < 2 
          {
          #else    
          if (RX_PORT | (1<<RX_PIN)) {
            RX_PORT &= (1<<RX_PIN);
            __asm__ __volatile__("nop"::);
          #endif
            if (rx_pin_state) {
              #if CHAR_SIZE < 8
              /* align rx.bits */
              rx.bits >>= (8 - CHAR_SIZE);
              #endif
              rx_stream.buffer[rx_stream.in] = rx.bits;
              WRAP(rx_stream.in, BUF_SIZE);
              ++rx_stream.size;
            }
          }
        }
        else
          /* frame error */;

        port_disable_rx_baud_timer();
        port_enable_rx_capture();
        break;
      }
      default: {
        rx.bits >>= 1;
        if (rx_pin_state)
          rx.bits |= 0x80;
        #if EVAL_PARITY
        rx.parity ^= (rx.bits & 0x80);
        #endif
      }
    }
    rx.n--;
  }
}

void setup_uart(unsigned long baud_rate) {
  if (baud_rate == 0) baud_rate = DEFAULT_BAUD_RATE;
  baud_timer_offset = ((unsigned int)(F_CPU * 1/baud_rate) / 2);

  /* tx state */
  tx_stream.in = tx_stream.out = tx_stream.size = 0;
  tx.bits = tx.n = 0;
  /* rx state */
  rx_stream.in = rx_stream.out = rx_stream.size = 0;
  rx.bits = rx.n = 0;
  /* tx io */
  TX_DDR  |= (1<<TX_PIN);
  TX_PORT |= (1<<TX_PIN);
  /* rx io */
  RX_DDR &= ~(1<<RX_PIN);
  RX_PORT &= ~(1<<RX_PIN);
  /* baud generator */
  /*
   * TIMER1
   *  CTC with OCR1A top
   *  OCR1A for baud generation
   *  resolution: F_CPU/baud
   */
  port_setup_baud_timer(F_CPU, baud_rate);
  /* rx capture */
  port_setup_rx_capture();
}

#define tx_free() \
  (tx_stream.size < BUF_SIZE)

void send(char c) {
  while(!tx_free());
  tx_stream.buffer[tx_stream.in] = (unsigned char)c;
  WRAP(tx_stream.in, BUF_SIZE);
  ++tx_stream.size;
}

char send2(char c) {
  return (tx_free() && ({
    tx_stream.buffer[tx_stream.in] = (unsigned char)c;
    WRAP(tx_stream.in, BUF_SIZE);
    ++tx_stream.size;
  }));
}

void send_str(const char *str) {
  while(*str) send(*str++);
}

#define rx_available() \
  (rx_stream.size > 0)

char recv() {
  while (!rx_available());
  char c = *(rx_stream.buffer + rx_stream.out);
  WRAP(rx_stream.out, BUF_SIZE);
  --rx_stream.size;
  return c;
}

char recv2() {
  return (rx_available() && ({
    char c = *(rx_stream.buffer + rx_stream.out);
    WRAP(rx_stream.out, BUF_SIZE);
    --rx_stream.size; c;
  }));
}

void recv_str(const char *str, int n) {
  char *p = (char*)str;
  while(--n) *(p++) = recv();
  *p = 0;
}
