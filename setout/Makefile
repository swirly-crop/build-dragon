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

$(PREFIX)/c_app: src/main.c src/helper.c | $(PREFIX)
	$(CC) $(CFLAGS) -o $@ src/main.c src/helper.c

$(PREFIX)/cxx_app: src/main.cpp src/util.cpp | $(PREFIX)
	$(CXX) $(CXXFLAGS) -o $@ src/main.cpp src/util.cpp

UNAME_M := $(shell uname -m)
ASM_SRCS :=
ifeq ($(UNAME_M),x86_64)
	ASM_SRCS := asm/fast_memcpy_x86_64.s
else ifeq ($(UNAME_M),amd64)
	ASM_SRCS := asm/fast_memcpy_x86_64.s
else ifeq ($(UNAME_M),aarch64)
	ASM_SRCS := asm/fast_memcpy_aarch64.s
else ifeq ($(UNAME_M),arm64)
	ASM_SRCS := asm/fast_memcpy_aarch64.s
else
	ASM_SRCS :=
endif

$(PREFIX)/asm_test: src/asm_test.c $(ASM_SRCS) | $(PREFIX)
	# Build only the assembler source appropriate for this host architecture
	$(CC) $(CFLAGS) -o $@ src/asm_test.c $(ASM_SRCS)

clean:
	rm -rf $(PREFIX) *.o

.PHONY: all clean
