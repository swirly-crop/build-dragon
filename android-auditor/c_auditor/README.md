# C Auditor

A C implementation of the Android auditor for performance comparison.

## Building

Run `make` to build the executables.

- `c_auditor`: Main auditor tool
- `sum`: Simple sum program for performance testing
- `sum_parallel`: Multi-threaded sum for parallel boost
- `sum_gpu`: Vulkan GPU accelerated sum (requires Vulkan SDK)

## GPU Acceleration

To build `sum_gpu.c`:
1. Install Vulkan SDK: `apt install libvulkan-dev vulkan-tools`
2. Install glslangValidator: `apt install glslang-tools`
3. Compile shader: `glslangValidator -V sum.comp -o sum.spv`
4. Embed SPIR-V binary in `sum_gpu.c` (replace placeholder)
5. Build: `gcc -O2 -lvulkan sum_gpu.c -o sum_gpu`