CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -std=c99
all: altimeter
altimeter: src/kalman.c src/main.c include/kalman.h
	$(CC) $(CFLAGS) -Iinclude src/kalman.c src/main.c -lm -o $@
clean:
	rm -f altimeter altimeter.exe test_filter test_filter.exe *.o
.PHONY: all test clean
test:
	$(CC) $(CFLAGS) -Iinclude src/kalman.c tests/test_filter.c -lm -o test_filter
	./test_filter

