CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
CFLAGS=-mfloat-abi=soft -mcpu=cortex-m4 -O0 -g3 -gdwarf-5 -mthumb 
INCLUDE=-I inc
OBJS = main.o startup.o


f439zi.elf: $(OBJS)
	$(CC) -T f439zi.ld -o $@ $(OBJS) -lnosys -nostdlib -Xlinker -Map=f439zi.map

startup.o: src/startup.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ src/startup.c

main.o: src/main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ src/main.c

clean:
	-rm -f $(OBJS)
