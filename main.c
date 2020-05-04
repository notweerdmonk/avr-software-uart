#include <uart.h>

#define EMIT_TRIGGER

char *pattern1 = "UUUU";
char *pattern2 = "AAAAAAAAAAAAAAAA";
char *pattern3 = "aaaaaaaaaaaaaaaa";
char *pattern4 = "abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main() {
  char *teststring = pattern4;

  setup_uart(0);
  sei();

#ifdef EMIT_TRIGGER
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
  DDRB |= 0x10;
  PORTB |= 0x10;;
#endif

  send_str(teststring);
  send('\n'); send('\r');

  /* loopback */
  for(;;send(recv()));

  return 0;
}
