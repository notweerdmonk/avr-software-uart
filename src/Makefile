
.DEFAULT_GOAL = all

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

COMPILE = avr-gcc -mmcu=$(DEVICE) -Wall -Wfatal-errors -Os $(CFLAGS) -DF_CPU=$(CLOCK) $(INCLUDE)
ifneq ($(strip $(DEBUG)),)
COMPILE := avr-gcc -mmcu=$(DEVICE) -g -save-temps -Wall -Wfatal-errors -Os $(CFLAGS) -DF_CPU=$(CLOCK) $(INCLUDE)
endif

.c.o: 
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

$(OBJECTS): $(SOURCES) $(INCLUDE_HEADERS)

all: $(OBJECTS)
	avr-ar rc libuart.a $(OBJECTS)

clean:
	rm -f uart.i uart.s libuart.a $(OBJECTS)

disasm: $(OBJECTS)	
	for obj in $^; do avr-objdump -d $obj > $(obj:.o=.lst); done

disasmall: $(OBJECTS)	
	for obj in $^; do avr-objdump -D $obj > $(obj:.o=.lst); done

cpp: $(SOURCES)
	for s in $^; do $(COMPILE) -E $s; done

%.lst: %.c
	$(COMPILE) -c -g -Wa,-adhln $< > $@

