.text
.global fast_memcpy_aarch64
.type fast_memcpy_aarch64, %function
fast_memcpy_aarch64:
    // aarch64: x0=dst, x1=src, x2=n
    cbz x2, 1f
    mov x3, x1
    mov x4, x0
    lsr x5, x2, #3
    cbz x5, 2f
0:
    ldr x6, [x3], #8
    str x6, [x4], #8
    subs x5, x5, #1
    b.ne 0b
2:
    and x6, x2, #7
    cbz x6, 1f
3:
    ldrb w7, [x3], #1
    strb w7, [x4], #1
    subs x6, x6, #1
    b.ne 3b
1:
    ret

/* mark stack non-executable */
.section .note.GNU-stack,"",@progbits
