# You need to set TARGET, MCU and SOURCES for your project.
# TARGET is the name of the executable file to be produced 
# $(TARGET).elf $(TARGET).hex and $(TARGET).txt nad $(TARGET).map are all generated.
# The TXT file is used for BSL loading, the ELF can be used for JTAG use

TARGET     = out
MCU        = msp430fr5969

SOURCES = interrupt.c

INCLUDES = -I/usr/local/msp430/include
CFLAGS   = -mmcu=$(MCU) -g -Wall -Wunused $(INCLUDES)   
ASFLAGS  = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map

CC       = msp430-gcc
LD       = msp430-gcc
AS       = msp430-gcc
OBJCOPY  = msp430-objcopy
SIZE     = msp430-size
MAKETXT  = srec_cat
CP       = cp -p
RM       = rm -f

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET).elf $(TARGET).hex $(TARGET).txt 

$(TARGET).elf: $(OBJECTS)
	echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
	echo ">>>> Size of Firmware <<<<"
	$(SIZE) $(TARGET).elf
	echo

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.txt: %.hex
	$(MAKETXT) -O $@ -TITXT $< -I
	unix2dos $(TARGET).txt
#  The above line is required for the DOS based TI BSL tool to be able to read the txt file generated from linux/unix systems.

%.o: %.c
	echo "Compiling $<"
	$(CC) -c $(CFLAGS) -o $@ $<
# rule for making assembler source listing, to see the code

%.lst: %.c
	$(CC) -c $(ASFLAGS) -Wa,-anlhd $< > $@

.SILENT:
.PHONY:	clean
clean:
	-$(RM) $(OBJECTS)
	-$(RM) $(TARGET).*
	-$(RM) $(SOURCES:.c=.lst)
