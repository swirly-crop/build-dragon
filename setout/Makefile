CC = gcc
CXX = g++
AS = as
AR = ar
CFLAGS = -O2 -Wall
CXXFLAGS = -O2 -Wall -std=gnu++17
ASMFLAGS =
PREFIX = bin

all: $(PREFIX)/c_app $(PREFIX)/cxx_app $(PREFIX)/asm_test

$(PREFIX):
	mkdir -p $(PREFIX)

$(PREFIX)/c_app: setout/src/main.c setout/src/helper.c | $(PREFIX)
	$(CC) $(CFLAGS) -o $@ setout/src/main.c setout/src/helper.c

$(PREFIX)/cxx_app: setout/src/main.cpp setout/src/util.cpp | $(PREFIX)
	$(CXX) $(CXXFLAGS) -o $@ setout/src/main.cpp setout/src/util.cpp

$(PREFIX)/asm_test: setout/src/asm_test.c setout/asm/fast_memcpy_x86_64.s setout/asm/fast_memcpy_aarch64.s | $(PREFIX)
	# Prefer assembling appropriate arch; build both and let linker drop unused code
	$(CC) $(CFLAGS) -o $@ setout/src/asm_test.c setout/asm/fast_memcpy_x86_64.s setout/asm/fast_memcpy_aarch64.s

clean:
	rm -rf $(PREFIX) *.o

.PHONY: all clean
