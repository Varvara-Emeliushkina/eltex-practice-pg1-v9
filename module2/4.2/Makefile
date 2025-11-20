CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = priority_queue
SOURCES = main.c priority_queue.c ui.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.c priority_queue.h ui.h
	$(CC) $(CFLAGS) -c main.c

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) $(CFLAGS) -c priority_queue.c

ui.o: ui.c ui.h priority_queue.h
	$(CC) $(CFLAGS) -c ui.c

clean:
	rm -f $(OBJECTS) $(TARGET)

all: $(TARGET)

.PHONY: clean all