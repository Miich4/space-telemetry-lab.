CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = space_telemetry
SOURCES = src/main.c ../common/telemetry.c

all: $(TARGET)

$(TARGET): $(SOURCES) ../common/telemetry.h
	$(CC) $(CFLAGS) -I../common $(SOURCES) -lm -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
