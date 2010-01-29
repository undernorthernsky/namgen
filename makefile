CC      := gcc
INC     := -Iinclude
FLAGS   := -Wall -pedantic --std=c99 -Os -ggdb
DEFS    := -DNDEBUG
CFLAGS  := $(INC) $(FLAGS) $(DEFS)
LDFLAGS := -Llib -lconfuse -lngtemplate -luseful

.PHONY: all clean

all: namgen

NAMGEN_SRC = namgen.c
NAMGEN_OBJ = $(NAMGEN_SRC:%.c=%.o)

namgen: $(NAMGEN_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f namgen *.o
