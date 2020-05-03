
.DEFAULT_GOAL = all

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

COMPILE = gcc -Wall $(INCLUDE) $(CFLAGS)
ifneq ($(strip $(DEBUG)),)
COMPILE := gcc -g -Wall $(INCLUDE) $(CFLAGS)
endif

.c.o: 
	$(COMPILE) -c $< -o $@

$(OBJECTS): $(SOURCES) $(INCLUDE_HEADERS)

all: uart_test.elf

uart_test.elf : $(OBJECTS)
	$(COMPILE) -o uart_test.elf $(OBJECTS)

test_run: uart_test.elf
	./uart_test.elf

clean:
	rm -f $(OBJECTS) uart_test.elf
