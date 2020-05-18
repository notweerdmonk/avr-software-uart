#ifndef PORT_H
#define PORT_H

#if defined __AVR_ATmega328P__
#include <atmega328p.h>
#elif defined __AVR_ATmega2560__
#include <atmega2560.h>
#elif defined __AVR_ATmega32__
#include <atmega32.h>
#endif

#endif /* PORT_H */
