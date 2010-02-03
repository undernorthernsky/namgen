
TMPL := $(shell pwd)/sub_make.tmpl

CC      := gcc
INC     := -Iinclude
FLAGS   := -Wall -pedantic --std=c99 -Os -ggdb
DEFS    := -DDEBUG -DCOMPILED_IN_TEMPLATE_FILE=\"$(TMPL)\"
CFLAGS  := $(INC) $(FLAGS) $(DEFS)
LDFLAGS := -Llib -lconfuse -lngtemplate -luseful

.PHONY: all clean

all: namgen

NAMGEN_SRC = namgen.c
NAMGEN_OBJ = $(NAMGEN_SRC:%.c=%.o)

namgen: $(NAMGEN_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(NAMGEN_OBJ): lib/libngtemplate.a
lib/libngtemplate.a:
	./buildlib.sh

clean:
	rm -f namgen *.o
