# AVR Makefile

# Paths
AVR_PATH=/usr/local/Cellar/avr-gcc@9/9.4.0_1
AVR_INCLUDE=$(AVR_PATH)/avr/include
AVR_BINUTILS_PATH=/usr/local/bin

# Project Name
PROJECTNAME=salva

# MCU
MCU=attiny85
PROGRAMMER_MCU=t85

# Source files
PRJSRC=salva.c

# Optimization level (0, 1, 2, 3, s)
OPTLEVEL=s

# Compiler and tools
CC=$(AVR_PATH)/bin/avr-gcc
OBJCOPY=$(AVR_BINUTILS_PATH)/avr-objcopy
OBJDUMP=$(AVR_BINUTILS_PATH)/avr-objdump
SIZE=$(AVR_BINUTILS_PATH)/avr-size
AVRDUDE=avrdude
REMOVE=rm -f

# Compiler flags
CFLAGS=-I. -I$(AVR_INCLUDE) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

# Linker flags
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU) \
	-L$(AVR_PATH)/avr/lib -lm

# Target file
TRG=$(PROJECTNAME).out

# Hex file
HEXROMTRG=$(PROJECTNAME).hex 

# Object files
OBJDEPS=$(PRJSRC:.c=.o)

# Default target
all: $(TRG)

# Linking
$(TRG): $(OBJDEPS) 
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS)

# Compiling
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generate hex file
hex: $(HEXROMTRG)

%.hex: %.out
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

# Clean up
clean:
	$(REMOVE) $(TRG) $(TRG).map
	$(REMOVE) $(PROJECTNAME).o $(PROJECTNAME).lst $(PROJECTNAME).out $(PROJECTNAME).out.map
	@echo "Cleaned all files except $(HEXROMTRG)"

# Add a new target for full cleanup including the hex file
distclean: clean
	$(REMOVE) $(HEXROMTRG)
	@echo "Removed all generated files including $(HEXROMTRG)"

# Update the .PHONY line
.PHONY: all clean distclean hex