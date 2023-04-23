C=gcc

C_SOURCE=$(shell find ../source -name *.c)
C_OBJECTS=$(addsuffix .o, $(C_SOURCE))
C_FLAGS=-O0 -I../include -Wall -Wextra

all:
	$(MAKE) $(C_OBJECTS)
	$(C) $(C_OBJECTS) -Wall -Wextra -o dark.elf
	./dark.elf
	$(MAKE) clean

%.c.o:	%.c
	$(C) -c $< $(C_FLAGS) -o $@

clean:
	rm -f $(C_OBJECTS) *.elf