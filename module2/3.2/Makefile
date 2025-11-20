CC = gcc
CFLAGS = -Wall
TARGET = network
SOURCES = main.c ip.c network.c ui.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) 
	
run: $(TARGET)
	./$(TARGET)

.PHONY: clean run