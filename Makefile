# TODO: make dependencies include header files
#       use gcc -MM to generate list of dependencies

SOURCES = \
	main.c \
	monopoly.c \
	chance.c \
	community.c \

TARGET  = monopoly

################

.PHONY: all clean

all: target

clean:
	rm -rf *.o *.exe

################

CC      = gcc
CFLAGS  = -c -Wall -ansi -pedantic
LDFLAGS =

OBJECTS = $(SOURCES:.c=.o)

.PHONY: target

target: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
