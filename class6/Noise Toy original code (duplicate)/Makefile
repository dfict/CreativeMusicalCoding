# AVR-GCC Makefile
PROJECT=${project}
SOURCES=$(PROJECT).c
PROGRAMMER=stk500v2
PROGRAMMER_PORT=/dev/cu.usbserial-A9006MOA
HIGH_FUSE=0xDF
LOW_FUSE=0xE2
CC=avr-gcc
OBJCOPY=avr-objcopy
MMCU=attiny85
PROCESSOR_SHORT=t85

CFLAGS=-mmcu=$(MMCU) -Wall

all: program

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(PROJECT).out $(PROJECT).hex
	rm $(PROJECT).out

$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) -I./ -Os -o $(PROJECT).out $(SOURCES)

program: $(PROJECT).hex
	avrdude -p $(PROCESSOR_SHORT) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -e -U hfuse:w:$(HIGH_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U flash:w:$(PROJECT).hex
clean:
	rm -f $(PROJECT).out
	rm -f $(PROJECT).hex