.section .text
.global sum_array
sum_array:
    # x86_64: sum array of long long
    # rdi = arr, rsi = n
    # return sum
    xor %rax, %rax
    test %rsi, %rsi
    jz .done
.loop:
    add (%rdi), %rax
    add $8, %rdi
    dec %rsi
    jnz .loop
.done:
    ret

.global sum_array_simd
sum_array_simd:
    # SIMD version using AVX2
    # rdi = arr, rsi = n
    vpxor %ymm0, %ymm0, %ymm0
    mov %rsi, %rcx
    shr $2, %rcx  # n / 4
    test %rcx, %rcx
    jz .scalar
.simd_loop:
    vpaddq (%rdi), %ymm0, %ymm0
    add $32, %rdi
    dec %rcx
    jnz .simd_loop
    # horizontal sum
    vextracti128 $1, %ymm0, %xmm1
    vpaddq %xmm1, %xmm0, %xmm0
    vpshufd $0x4e, %xmm0, %xmm1
    vpaddq %xmm1, %xmm0, %xmm0
    vmovq %xmm0, %rax
    # remaining elements
    mov %rsi, %rcx
    and $3, %rcx
.scalar:
    test %rcx, %rcx
    jz .done_simd
.scalar_loop:
    add (%rdi), %rax
    add $8, %rdi
    dec %rcx
    jnz .scalar_loop
.done_simd:
    ret

/* mark stack non-executable */
.section .note.GNU-stack,"",@progbits