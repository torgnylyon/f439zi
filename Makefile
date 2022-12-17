CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld

CFLAGS= \
-mfloat-abi=soft -mcpu=cortex-m4 -O0 -g3 -gdwarf-5 -mthumb \
-Wall -Wextra -Wsign-conversion -Wconversion -Wpedantic -std=gnu18

INC= \
-I inc

SRC = \
src/main.c \
src/startup.c \
src/clock.c

all:
	$(CC) -T f439zi.ld -o f439zi.elf $(SRC) $(CFLAGS) $(INC) -nostdlib -Xlinker -Map=f439zi.map

.PHONY: clean
clean:
	-rm -f f439zi.elf f439zi.map
