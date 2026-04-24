CC = gcc
CFLAGS = -O2 -mavx2 -pthread
TARGET = c_auditor
SUM_TARGET = sum
PARALLEL_TARGET = sum_parallel
GPU_TARGET = sum_gpu

all: $(TARGET) $(SUM_TARGET) $(PARALLEL_TARGET) $(GPU_TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

$(SUM_TARGET): sum.c
	$(CC) $(CFLAGS) -o $(SUM_TARGET) sum.c

$(PARALLEL_TARGET): sum_parallel.c
	$(CC) $(CFLAGS) -o $(PARALLEL_TARGET) sum_parallel.c

$(GPU_TARGET): sum_gpu.c
	$(CC) $(CFLAGS) -lvulkan -o $(GPU_TARGET) sum_gpu.c || echo "Vulkan not available, skipping GPU build"

clean:
	rm -f $(TARGET) $(SUM_TARGET) $(PARALLEL_TARGET) $(GPU_TARGET)