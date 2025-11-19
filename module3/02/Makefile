CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGETS = myshell sum concat maxlen

all: $(TARGETS)

myshell: myshell.c
	$(CC) $(CFLAGS) -o myshell myshell.c

sum: sum.c
	$(CC) $(CFLAGS) -o sum sum.c

concat: concat.c
	$(CC) $(CFLAGS) -o concat concat.c

maxlen: maxlen.c
	$(CC) $(CFLAGS) -o maxlen maxlen.c

clean:
	rm -f $(TARGETS)

.PHONY: all test clean