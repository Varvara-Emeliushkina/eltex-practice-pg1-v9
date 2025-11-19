CC = gcc
CFLAGS = -Wall
TARGET = phonebook
SOURCES = main.c phonebook.c contact.c ui.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) phonebook.dat

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run