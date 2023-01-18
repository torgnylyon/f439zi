CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
DEBUG_ENABLED = YES

CFLAGS = -mfloat-abi=hard -mcpu=cortex-m4 -mthumb \
-Wall -Wextra -Wsign-conversion -Wconversion -Wpedantic -std=gnu18

ifeq ($(DEBUG_ENABLED), YES)
	CFLAGS += -O0 -g3 -gdwarf-5
else
	CFLAGS += -O2 -g0
endif

INC = \
-iquote inc

SRC = \
src/main.c \
src/startup.c \
src/clock.c \
src/timdma.c \
src/flash.c

all:
	$(CC) -T f439zi.ld -o f439zi.elf $(SRC) $(CFLAGS) $(INC) -nostdlib -Xlinker -Map=f439zi.map

.PHONY: clean
clean:
	-rm -f f439zi.elf f439zi.map
