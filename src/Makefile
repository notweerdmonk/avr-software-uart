# MIT License
# 
# Copyright (c) 2020 - 2022 notweerdmonk
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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

