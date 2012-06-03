.PHONY: all clean rclean

# if we have no config, the default it to create one
TARGET := make.conf
# thus don't error on missing conf
-include make.conf

all: $(TARGET)

make.conf:
	./configure

TMPL_1 := sub_make.tmpl

ifdef DEBUG
DEBUG_FLAG := -ggdb
else
DEBUG_FLAG := -DNDEBUG
endif

CC      := gcc
INC     := -Iinclude
FLAGS   := -Wall --std=c99 -Os
DEFS    := $(DEBUG_FLAG) $(ZIP_FLAG) -D_GNU_SOURCE -DCOMPILED_IN_TEMPLATE_FILE=\"$(TMPL_1)\"
CFLAGS  := $(INC) $(FLAGS) $(DEFS)
LDFLAGS := -g -rdynamic -Llib -lngtemplate -luseful $(FLEX_LIB) $(ZIP_LIB)


ZIP_DATA   := $(wildcard zip_data/*)
NAMGEN_SRC = namgen.c myio.c target.c src_gatherer.c template.c \
			    dirscanner.c logging.c bt.c lex.yy.c y.tab.c
NAMGEN_OBJ = $(NAMGEN_SRC:%.c=%.o)

ifdef UPDATE_GRAMMAR
lex.yy.c: rules.l
	flex rules.l

y.tab.c y.tab.h: rules.y
	yacc -d rules.y
endif

namgen.bin: $(NAMGEN_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

namgen: namgen.bin data.zip
	cat namgen.bin data.zip > namgen
	chmod +x namgen
	zip -A namgen

lex.yy.o: lex.yy.c y.tab.h

data.zip: $(ZIP_DATA)
	(cd zip_data && zip ../$@ *)

$(NAMGEN_OBJ): lib/libngtemplate.a
lib/libngtemplate.a:
	./buildlib.sh

clean:
	rm -f namgen.bin *.o data.zip

rclean: clean
	rm -f namgen lib/* make.conf
