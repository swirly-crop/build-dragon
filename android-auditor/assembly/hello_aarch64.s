.section .data
msg: .ascii "Android Auditor ARM64\n"
len = . - msg

.section .text
.global _start
_start:
    // write(1, msg, len)
    mov x0, #1
    adr x1, msg
    mov x2, #len
    mov x8, #64
    svc #0

    // exit(0)
    mov x0, #0
    mov x8, #93
    svc #0

.global neon_memcpy
.type neon_memcpy, %function
neon_memcpy:
    // aarch64 calling: x0 = dst, x1 = src, x2 = n
    cbz x2, 1f
    mov x3, x1
    mov x4, x0
    // number of 16-byte blocks
    lsr x5, x2, #4
    cbz x5, 2f
0:
    ld1 {v0.16b}, [x3], #16
    st1 {v0.16b}, [x4], #16
    subs x5, x5, #1
    b.ne 0b
2:
    and x6, x2, #15
    cbz x6, 1f
    // copy remaining bytes
    mov x7, x3
    mov x8, x4
3:
    ldrb w9, [x7], #1
    strb w9, [x8], #1
    subs x6, x6, #1
    b.ne 3b
1:
    ret

/* mark stack non-executable */
.section .note.GNU-stack,"",@progbits

