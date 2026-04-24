CC = gcc
ASFLAGS =
PREFIX = bin

all: x86_64 aarch64

x86_64: $(PREFIX)
	# build native x86_64 executable (host must be x86_64)
	$(CC) -nostdlib -static -o $(PREFIX)/hello hello.s || true
	$(CC) -nostdlib -static -o $(PREFIX)/add_sum add_sum.s || true
	$(CC) -O2 -mavx2 -o $(PREFIX)/test_sum test_sum.c add_sum.s

aarch64: $(PREFIX)
	# cross-build with aarch64-linux-gnu-gcc if available
	if command -v aarch64-linux-gnu-gcc >/dev/null 2>&1; then \
		aarch64-linux-gnu-gcc -nostdlib -static -o $(PREFIX)/hello_aarch64 hello_aarch64.s; \
		aarch64-linux-gnu-gcc -nostdlib -static -o $(PREFIX)/add_sum_aarch64 add_sum_aarch64.s; \
	else \
		echo "aarch64-linux-gnu-gcc not found; try Android NDK or install cross compiler"; exit 0; \
	fi

$(PREFIX):
	mkdir -p $(PREFIX)

clean:
	rm -rf $(PREFIX)

.PHONY: all x86_64 aarch64 clean