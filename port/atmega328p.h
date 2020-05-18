#ifndef PORT_ATMEGA32P_H
#define PORT_ATMEGA32P_H

#define PORT_TX_PIN  PB1
#define PORT_TX_DDR  DDRB
#define PORT_TX_PORT PORTB
#define PORT_TX_INP  PINB

#define PORT_RX_PIN  PD7
#define PORT_RX_DDR  DDRD
#define PORT_RX_PORT PORTD
#define PORT_RX_INP  PIND

#define PORT_RX_INT_EN  PCIE2
#define PORT_RX_INT_PIN PCINT23
#define PORT_RX_INT_MSK PCMSK2

#define PORT_BAUD_TIMER_COUNT TCNT1
#define PORT_BAUD_TIMER_TOP   OCR1B

#define port_rx_capture_isr PCINT2_vect

#define port_rx_isr TIMER1_COMPB_vect

#define port_tx_isr TIMER1_COMPA_vect

#define port_enable_rx_capture() \
  PCICR  |= (1<<RX_INT_EN)

#define port_disable_rx_capture() \
  PCICR  &= ~(1<<RX_INT_EN)

#define port_setup_rx_capture() \
  do { \
    port_enable_rx_capture(); \
    RX_INT_MSK |= (1<<RX_INT_PIN); \
  } while(0)

#define port_clear_rx_capture_isr() \
  TIFR1 |= (1<<OCF1B);

#define port_enable_rx_baud_timer() \
  TIMSK1 |= (1<<OCIE1B)

#define port_disable_rx_baud_timer() \
  TIMSK1 &= ~(1<<OCIE1B)

#define port_setup_baud_timer(fcpu, baudrate) \
  do { \
    TIMSK1 |= (1<<OCIE1A); \
    TCCR1A = 0; \
    TCCR1B = (1<<WGM12) | (1<<CS10); \
    TCCR1C = 0; \
    TCNT1  = 0; \
    OCR1A  = (unsigned int)(fcpu * 1/baudrate + 0.5); \
  } while(0)

#endif /* PORT_ATMEGA32P_H */
