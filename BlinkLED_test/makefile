CPU = cortex-m4
ARCH = arm7e-m
LINKER_SCRIPT = -Wl,-T"tm4c1294ncpdt.ld"

LIBRARY = -L"C:\Program Files (x86)\GNU Tools Arm Embedded\9 2019-q4-major\arm-none-eabi\lib"

CFLAGS = -c -mthumb -mfloat-abi=soft -mcpu=$(CPU) -Wall $(INCLUDES) \
	 #-specs="nosys.specs"

LDFLAGS = -mcpu=$(CPU) $(LINKER_SCRIPT) $(INCLUDES) -Wl,-Map=main.map \
	 -specs="nosys.specs"
INCLUDES = -I./Library -I./Library/Keil
CC = arm-none-eabi-gcc
EXE = main.out
HEX = main.hex

all: main.o handy.o tm4c1294ncpdt_startup.o $(EXE) $(HEX)

%.o : %.c
	$(CC) $(CFLAGS) $^ -o $@

handy.o: ./Library/handy.c
	$(CC) $(CFLAGS) $^ -o $@

UART.o: ./Library/UART.c
	$(CC) $(CFLAGS) $^ -o $@

$(EXE): main.o handy.o UART.o tm4c1294ncpdt_startup.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBRARY)

$(HEX): $(EXE)
	arm-none-eabi-objcopy $^ -O ihex $@

clean :
	rm -rf *.o *.hex *.out *.map

connect:
	openocd -f "C:\Users\Diaa Eldeen\Desktop\xpack-openocd-0.10.0-14\scripts\board\ti_ek-tm4c1294xl.cfg"

