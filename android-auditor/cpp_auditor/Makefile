CC = g++
CFLAGS = -std=c++17 -O2 -pthread
TARGET = cpp_auditor
SUM_TARGET = sum
FIXER_TARGET = realtime_fixer

all: $(TARGET) $(SUM_TARGET) $(FIXER_TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp

$(SUM_TARGET): sum.cpp
	$(CC) $(CFLAGS) -o $(SUM_TARGET) sum.cpp

$(FIXER_TARGET): realtime_fixer.cpp safe_sum.s
	$(CC) $(CFLAGS) -o $(FIXER_TARGET) realtime_fixer.cpp safe_sum.s

clean:
	rm -f $(TARGET) $(SUM_TARGET) $(FIXER_TARGET)
