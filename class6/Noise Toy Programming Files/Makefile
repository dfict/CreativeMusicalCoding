# Constants
PROCESSOR_T85=t85
HIGH_FUSE_T85_Default=0xDF
LOW_FUSE_T85_NoCKDIV8=0xE2

# Hardware Configuration
PROCESSOR=$(PROCESSOR_T85)
HIGH_FUSE=$(HIGH_FUSE_T85_Default)
LOW_FUSE=$(LOW_FUSE_T85_NoCKDIV8)

# Project Configuration
PROJECT=${project}
FILE_NAME=$(PROJECT)
DIRECTORY=$(PROJECT)
HEX_PATH=$(DIRECTORY)/$(FILE_NAME).hex

# Programmer
ifneq ($(wildcard /dev/cu.usbserial-A*),) 
    PROGRAMMER=stk500v2
    PROGRAMMER_PORT=/dev/cu.usbserial-A*
else 
    PROGRAMMER=usbasp
    PROGRAMMER_PORT=usb
endif


all: program

program:
	avrdude -p $(PROCESSOR) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -e -U hfuse:w:$(HIGH_FUSE):m -U lfuse:w:$(LOW_FUSE):m -U flash:w:$(HEX_PATH)
	