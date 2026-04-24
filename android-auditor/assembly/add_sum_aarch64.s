.section .text
.global sum_array
sum_array:
    // aarch64: sum array of long long
    // x0 = arr, x1 = n
    // return sum
    mov x2, #0
    cbz x1, .done
.loop:
    ldr x3, [x0], #8
    add x2, x2, x3
    subs x1, x1, #1
    b.ne .loop
.done:
    mov x0, x2
    ret

.global sum_array_simd
sum_array_simd:
    // NEON version
    // x0 = arr, x1 = n
    mov x2, #0
    dup v0.2d, x2  // zero vector
    mov x3, x1
    lsr x3, x3, #1  // n / 2
    cbz x3, .scalar
.simd_loop:
    ld1 {v1.2d}, [x0], #16
    add v0.2d, v0.2d, v1.2d
    subs x3, x3, #1
    b.ne .simd_loop
    // horizontal sum
    addp d0, v0.2d
    mov x2, v0.d[0]
    // remaining
    tst x1, #1
    b.eq .done_simd
.scalar:
    ldr x3, [x0]
    add x2, x2, x3
.done_simd:
    mov x0, x2
    ret