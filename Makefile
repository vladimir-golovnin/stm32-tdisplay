CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy


SFLAGS= --static -nostartfiles -std=c11 -g3 -Os
SFLAGS+= -fno-common -ffunction-sections -fdata-sections
SFLAGS+= -I./libopencm3/include -L./libopencm3/lib
LFLAGS+=-Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

M3_FLAGS= $(SFLAGS) -mcpu=cortex-m3 -mthumb -msoft-float

LFLAGS_STM32=$(LFLAGS) main.c delay.c lcd1602.c sys.c -T ld.stm32.basic

STM32F1_CFLAGS=$(M3_FLAGS) -DSTM32F1 $(LFLAGS_STM32) -lopencm3_stm32f1


all: flash

build: out.bin

out.bin: out.elf
	$(OBJCOPY) -Obinary out.elf out.bin

out.elf: main.c delay.c delay.h lcd1602.h lcd1602.c sys.c libopencm3/lib/libopencm3_stm32f1.a
	$(CC) $(STM32F1_CFLAGS) -o out.elf

libopencm3/lib/libopencm3_stm32f1.a: 
	make -C libopencm3

flash: out.bin
	st-flash write out.bin 0x08000000
	@date > flash
