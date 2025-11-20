CC = gcc
CFLAGS = -Wall
TARGET = phonebook_tree
SOURCES = main.c phonebook.c contact.c ui.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGETS)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run