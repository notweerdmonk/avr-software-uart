
.DEFAULT_GOAL = all

DEVICE = atmega328p
CLOCK = 16000000
export

ifeq ($(DEVICE),atmega2560)
PROGRAMMER = wiring
AVRDUDE_OPTIONS = -v -D
else
PROGRAMMER = arduino
AVRDUDE_OPTIONS = -v
endif
PORT = /dev/ttyACM*
AVRDUDE = avrdude $(AVRDUDE_OPTIONS) -c $(PROGRAMMER) -P $(PORT) -p $(DEVICE)

FUSES = -U lfuse:w:0x64:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

INCLUDE_DIRS = . include src port 
INCLUDE = $(foreach i, $(INCLUDE_DIRS),-I$(CURDIR)/$i)
INCLUDE_HEADERS = $(foreach dir, $(abspath $(INCLUDE_DIRS)), $(wildcard $(dir)/*.h))

LIB_DIR = src
LIB_SOURCES = $(wildcard $(LIB_DIR)/*.c)
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)

TEST_DIR = tests
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

ifneq ($(strip $(SIM)),)
override CFLAGS += -DSIMULATION -DDEVICE_NAME=$(DEVICE)
endif

LDFLAGS = -L$(LIB_DIR)

LDLIBS = -luart

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

$(LIB_OBJECTS): $(LIB_DIR)
	$(MAKE) -C $<

$(TEST_OBJECTS): $(TEST_DIR)
	$(MAKE) -C $<

test: $(TEST_OBJECTS)

lib: $(LIB_OBJECTS)

all: main.hex $(TEST_OBJECTS)
	avr-size -C --mcu=$(DEVICE) main.elf

main.elf: $(OBJECTS) $(LIB_OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS) $(LDFLAGS) $(LDLIBS)

# TODO: add EEPROM section
main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

flash: all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash

# TODO: flash bootloader
load: all
	@echo Nothing to do

clean:
	rm -f main.s main.i main.hex main.elf main.lst $(OBJECTS)
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(TEST_DIR) clean

# preprocess only
cpp:
	$(COMPILE) -E $(SOURCES)

# disassembly
disasm:	main.elf
	avr-objdump -S -d main.elf > main.lst

disasmall: main.elf
	avr-objdump -S -D main.elf > main.lst

# assembly lisiting
%.s: %.c
	$(COMPILE) -c -g -Wa,-adhln $< > $@

# distribution
dist:
	@make -s clean
	tar --exclude=.gdb* --exclude=peda-session-*.txt --exclude=.*.swp --exclude-backups --exclude-vcs --exclude-vcs-ignore -czvf $(shell basename $(shell pwd)).tar.gz ./*

# simluation
sim-gdb:
ifneq ($(strip $(SIMAVR)),)
	$(SIMAVR) -v -v -v -g -m $(DEVICE) -f $(CLOCK) main.elf
else
	@echo simavr not found
endif

sim-trace:
ifneq ($(strip $(SIMAVR)),)
	$(SIMAVR) -v -v -v -t -m $(DEVICE) -f $(CLOCK) main.elf
else
	@echo simavr not found
endif
