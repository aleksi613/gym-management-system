CC := gcc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Wall -Wextra -pedantic
TARGET := GymMS2.exe
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@

clean:
	$(RM) $(TARGET)
