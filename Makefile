### FILES ###

# executable name
TARGET=colorLines
# sources names
SOURCES=error utils playGame game colorsBack colors menu userInterface colorsFront

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
all: release

OBJS=$(addsuffix .o,$(SOURCES) )
OUTPUT_OPTION=-o $(OPATH)$@
LDFLAGS += -L $(OPATH)
CFLAGS += $(addprefix -I ,$(HPATH))
vpath %.c $(CPATH)

.PHONY: all clean debug release

$(TARGET): $(OBJS)
	$(CC) $(TFLAGS) -o $(TARGET) $(addprefix $(OPATH),$(OBJS))

debug: override CFLAGS += $(DEBUGFLAGS)
debug: override TFLAGS += $(DEBUGFLAGS)
debug: dir $(TARGET)

release: override CFLAGS += $(RELEASEFLAGS)
release: override TFLAGS += $(RELEASEFLAGS)
release: dir $(TARGET)

clean:
	rm -f $(OPATH)*.o
	rm -f $(TARGET)
	find ./ -name "*~" -delete

dir:
	mkdir -p $(OPATH)
