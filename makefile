USE_ZIP := 1
DEBUG := 1
TEMPLATES := sub_make.tmpl

ifdef USE_ZIP
TMPL_1 := sub_make.tmpl
ZIP_FLAG := -DUSE_ZIP
ZIP_LIBS := -lzzip -lz
TARGET := namgen
else
TMPL_1 := $(shell pwd)/sub_make.tmpl
TARGET := namgen.bin
endif

ifdef DEBUG
DEBUG_FLAG := -ggdb
else
DEBUG_FLAG := -DNDEBUG
endif

CC      := gcc
INC     := -Iinclude
FLAGS   := -Wall -pedantic --std=c99 -Os
DEFS    := $(DEBUG_FLAG) $(ZIP_FLAG) -D_GNU_SOURCE -DCOMPILED_IN_TEMPLATE_FILE=\"$(TMPL_1)\"
CFLAGS  := $(INC) $(FLAGS) $(DEFS)
LDFLAGS := -Llib -lngtemplate -luseful -ll $(ZIP_LIBS)

.PHONY: all clean

all: $(TARGET)

NAMGEN_SRC = namgen.c myio.c target.c src_gatherer.c template.c dirscanner.c bt.c lex.yy.c y.tab.c
NAMGEN_OBJ = $(NAMGEN_SRC:%.c=%.o)

lex.yy.c: rules.l
	flex rules.l

y.tab.c y.tab.h: rules.y
	yacc -d rules.y

namgen.bin: $(NAMGEN_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

namgen: namgen.bin data.zip
	cat namgen.bin data.zip > namgen
	chmod +x namgen
	zip -A namgen

lex.yy.o: lex.yy.c y.tab.h

data.zip: $(TEMPLATES) usage.txt documentation.txt
	zip $@ $+

$(NAMGEN_OBJ): lib/libngtemplate.a
lib/libngtemplate.a:
	./buildlib.sh

clean:
	rm -f namgen namgen.bin *.o lex.yy.c y.tab.c y.tab.h data.zip
