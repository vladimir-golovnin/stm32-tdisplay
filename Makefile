CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

SFLAGS=-std=c11 -g0 -Os -fno-common -ffunction-sections -fdata-sections
SFLAGS+= -I./libopencm3/include -L./libopencm3/lib
M3_FLAGS= $(SFLAGS) -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1 

LFLAGS=--static -nostartfiles 
LFLAGS+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

LFLAGS_STM32=$(LFLAGS) *.o -T ld.stm32.basic

STM32F1_CFLAGS=$(M3_FLAGS) $(LFLAGS_STM32) -lopencm3_stm32f1


all: flash

build: out.bin

out.bin: out.elf
	$(OBJCOPY) -Obinary out.elf out.bin

out.elf: main.o delay.o lcd1602.o sys.o display.o tsensor.o ds1621.o libopencm3/lib/libopencm3_stm32f1.a
	$(CC) $(STM32F1_CFLAGS) -o out.elf

libopencm3/lib/libopencm3_stm32f1.a: 
	make -C libopencm3

flash: out.bin
	st-flash write out.bin 0x08000000
	@date > flash

delay.o: delay.h delay.c
	$(CC) -c delay.c $(M3_FLAGS) -o delay.o

lcd1602.o: lcd1602.h lcd1602.c
	$(CC) -c lcd1602.c $(M3_FLAGS) -o lcd1602.o

sys.o: sys.c
	$(CC) -c sys.c $(M3_FLAGS) -o sys.o

display.o: display.h display.c lcd1602.h
	$(CC) -c display.c $(M3_FLAGS) -o display.o

tsensor.o: tsensor.h tsensor.c ds1621.h
	$(CC) -c tsensor.c $(M3_FLAGS) -o tsensor.o

ds1621.o: ds1621.h ds1621.c
	$(CC) -c ds1621.c $(M3_FLAGS) -o ds1621.o

main.o: main.c delay.h display.h tsensor.h
	$(CC) -c main.c $(M3_FLAGS) -o main.o

clean: 
	rm *.o out.elf out.bin flash
