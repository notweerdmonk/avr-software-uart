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
