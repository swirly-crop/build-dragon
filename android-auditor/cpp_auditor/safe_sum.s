.section .text
.global sum_array_scalar
sum_array_scalar:
    # Safe scalar sum
    # rdi = arr, rsi = n
    # return sum
    xor %rax, %rax
    test %rsi, %rsi
    jz .done_safe
.loop_safe:
    add (%rdi), %rax
    add $8, %rdi
    dec %rsi
    jnz .loop_safe
.done_safe:
    ret

.global sum_array_simd
sum_array_simd:
    # SIMD version (assume AVX2 available)
    vpxor %ymm0, %ymm0, %ymm0
    mov %rsi, %rcx
    shr $2, %rcx
    test %rcx, %rcx
    jz .scalar_simd
.simd_loop:
    vpaddq (%rdi), %ymm0, %ymm0
    add $32, %rdi
    dec %rcx
    jnz .simd_loop
    vextracti128 $1, %ymm0, %xmm1
    vpaddq %xmm1, %xmm0, %xmm0
    vpshufd $0x4e, %xmm0, %xmm1
    vpaddq %xmm1, %xmm0, %xmm0
    vmovq %xmm0, %rax
.scalar_simd:
    mov %rsi, %rcx
    and $3, %rcx
    test %rcx, %rcx
    jz .done_simd
.scalar_loop_simd:
    add (%rdi), %rax
    add $8, %rdi
    dec %rcx
    jnz .scalar_loop_simd
.done_simd:
    ret

/* mark stack non-executable */
.section .note.GNU-stack,"",@progbits