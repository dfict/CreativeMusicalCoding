# Makefile for zoo.c with inline functions

# Project name
PROJECTNAME = zoo

# Source file
SRC = $(PROJECTNAME).c

# MCU
MCU = attiny85

# Compiler
CC = /usr/local/Cellar/avr-gcc@9/9.4.0_1/bin/avr-gcc
OBJCOPY = /usr/local/bin/avr-objcopy
SIZE = /usr/local/bin/avr-size

# Flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=8000000UL -Os -Wall
CFLAGS += -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields
CFLAGS += -I/usr/local/Cellar/avr-gcc@9/9.4.0_1/avr/include
CFLAGS += -fno-inline-small-functions -fno-split-wide-types
CFLAGS += -fkeep-inline-functions -finline-limit=3 -fno-inline-functions
CFLAGS += -fno-move-loop-invariants -fno-tree-scev-cprop

LDFLAGS = -Wl,-Map,$(PROJECTNAME).map -mmcu=$(MCU)
LDFLAGS += -L/usr/local/Cellar/avr-gcc@9/9.4.0_1/avr/lib/avr25 -lm

# Targets
all: $(PROJECTNAME).hex

$(PROJECTNAME).elf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

$(PROJECTNAME).hex: $(PROJECTNAME).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	$(SIZE) $<

clean:
	rm -f *.elf *.hex *.map

.PHONY: all clean