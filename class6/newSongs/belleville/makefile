# Updated Makefile for belleville project

MCU=attiny85
PROGRAMMER_MCU=t85
PROJECTNAME=belleville
PRJSRC=belleville.c

# Optimization level, use s (size opt), 1, 2, 3 or 0 (off)
OPTLEVEL=s

INC=
LIBS=

# Compiler flags
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	-fno-inline-small-functions             \
	-fno-split-wide-types                   \
	-ffunction-sections -fdata-sections     \
	-fno-inline                             \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

# Linker flags
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU) \
	-Wl,--gc-sections                 \
	-L/usr/local/Cellar/avr-gcc@9/9.4.0_1/avr/lib/avr25 \
	-lm $(LIBS)

# Programs
CC=/usr/local/Cellar/avr-gcc@9/9.4.0_1/bin/avr-gcc
OBJCOPY=/usr/local/bin/avr-objcopy
OBJDUMP=/usr/local/bin/avr-objdump
SIZE=/usr/local/bin/avr-size
AVRDUDE=avrdude
REMOVE=rm -f

# Target file names
TRG=$(PROJECTNAME).out
DUMPTRG=$(PROJECTNAME).s
HEXROMTRG=$(PROJECTNAME).hex 

# Object files
OBJ=$(PRJSRC:.c=.o)

# Default target
all: $(TRG)

# Compile: create object files from C source files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Link: create ELF output file from object files
$(TRG): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Create final output files (.hex) from ELF output file
%.hex: %.out
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Listing of the final file
disasm: $(TRG)
	$(OBJDUMP) -d $(TRG)

# Target: clean project
clean:
	$(REMOVE) $(TRG) $(TRG).map $(DUMPTRG)
	$(REMOVE) $(OBJ)
	$(REMOVE) $(HEXROMTRG)

# Phony targets
.PHONY: all clean disasm