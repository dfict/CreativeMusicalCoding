# Constants
PROCESSOR_T85=t85
PROCESSOR_M168=m168
HIGH_FUSE_T85_Default=0xDF
LOW_FUSE_T85_NoCKDIV8=0xE2
MMCU_T85=attiny85
MMCU_M168=atmega328

# Hardware Configuration
processor=$(PROCESSOR_T85)
HIGH_FUSE=$(HIGH_FUSE_T85_Default)
LOW_FUSE=$(LOW_FUSE_T85_NoCKDIV8)

# Project Configuration
PROJECT=${project}
FILE_NAME=$(PROJECT)
HEX_PATH=$(FILE_NAME).hex

# Programmer and OS
ifeq ($(processor),$(PROCESSOR_T85))
	MMCU=$(MMCU_T85)
else
	MMCU=$(MMCU_M168)
endif
ifeq ($(os),windows)
	CLEAN_COMMAND=del /f
	PROGRAMMER=avrisp
	PROGRAMMER_PORT=COM5
	PROGRAMMER_BAUD_PARAMETER=-b 19200
else
	CLEAN_COMMAND=rm -f
	ifneq ($(wildcard /dev/cu.usbserial-A*),) 
		PROGRAMMER=stk500v2
		PROGRAMMER_PORT=/dev/cu.usbserial-A*
	else
		ifneq ($(wildcard /dev/cu.usbmodem*),) 
			PROGRAMMER=avrisp
			PROGRAMMER_PORT=/dev/cu.usbmodem*
			PROGRAMMER_BAUD_PARAMETER=-b 19200
		else
			PROGRAMMER=usbtiny
			PROGRAMMER_PORT=usb
		endif
	endif
endif

# Tools
CC=avr-gcc
OBJCOPY=avr-objcopy
SOURCES=$(PROJECT).c
CFLAGS=-mmcu=$(MMCU) -Wall
AVRDUDE=avrdude

# "all" target runs by default (compiles and programs the chip)
all: program clean

# "hex" target: copies .out to .hex
$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(PROJECT).out $(PROJECT).hex
	rm $(PROJECT).out

# "out" target: compiles .c to .out
$(PROJECT).out: $(SOURCES)
	@printf "\n`tput bold`Compiling...`tput sgr0`\n"
	$(CC) $(CFLAGS) -I./ -Os -o $(PROJECT).out $(SOURCES)

# "program" target: programs .hex onto the chip
program:
	@printf "\n`tput bold`Programming...`tput sgr0`\n"
	$(AVRDUDE) -p $(processor) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) $(PROGRAMMER_BAUD_PARAMETER) -e -U hfuse:w:$(HIGH_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U flash:w:$(HEX_PATH)

# "clean" target: removes .out and .hex files
clean:
	@printf "\n`tput bold`Cleaning up...`tput sgr0`\n"
	$(CLEAN_COMMAND) $(PROJECT).out
	$(CLEAN_COMMAND) $(PROJECT).hex
