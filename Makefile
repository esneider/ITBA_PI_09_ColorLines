### FILES ###

# executable name
TARGET=colorLines
# sources names
OBJS=error.o utils.o game.o commands.o colors.o menu.o test01.o

### DIRECTORIES ###

# .o files directories
OPATH=lib/
# .h files directories
HPATH=back_end/include front_end/include
# .c files directories
CPATH=back_end/source front_end/source

### COMPILATIO FLAGS ###

# sources flags
CFLAGS=-Wall -pedantic -std=c99 -lm
# target flags
TFLAGS=-Wall -pedantic -std=c99 -lm
# extra debug flags
DEBUGFLAGS=-g -O0
# extra release flags
RELEASEFLAGS=-O3

### OTHERS ###

# compiler
CC=gcc

### possible targets: debug release clean
all: debug


OUTPUT_OPTION=-o $(OPATH)$@
LDFLAGS += -L $(OPATH)
CFLAGS += $(addprefix -I ,$(HPATH))
vpath %.c $(CPATH)

.PHONY: all clean debug release

$(TARGET): $(OBJS)
	$(CC) $(TFLAGS) -o $(TARGET) $(addprefix $(OPATH),$(OBJS))

debug: override CFLAGS += $(DEBUGFLAGS)
debug: override TFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

release: override CFLAGS += $(RELEASEFLAGS)
release: override TFLAGS += $(RELEASEFLAGS)
release: $(TARGET)

clean:
	rm -f $(OPATH)*.o
	rm -f $(TARGET)
