CC = clang
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lcurl

all: ask

ask: main.c
	$(CC) $(CFLAGS) main.c -o ask $(LDFLAGS)

clean:
	rm -f ask

.PHONY: clean