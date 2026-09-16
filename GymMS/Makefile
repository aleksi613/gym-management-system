CC := gcc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Wall -Wextra -pedantic
TARGET := GymMS2.exe
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)
TEST_TARGET := gymms_tests.exe
TEST_SOURCES := tests/test_runner.c src/date_utils.c src/equipment.c src/member.c src/storage.c

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@

$(TEST_TARGET): $(TEST_SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_SOURCES) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	$(RM) $(TARGET) $(TEST_TARGET)
