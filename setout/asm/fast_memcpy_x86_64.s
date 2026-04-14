.intel_syntax noprefix
.text
.global fast_memcpy_x86_64
.type fast_memcpy_x86_64, @function
fast_memcpy_x86_64:
    /* x86_64 SysV: rdi=dst, rsi=src, rdx=n */
    test rdx, rdx
    jz .ret
    mov rcx, rdx
    shr rcx, 3          # number of qwords
.loop_qword:
    cmp rcx, 0
    je .tail
    mov rax, QWORD PTR [rsi]
    mov QWORD PTR [rdi], rax
    add rsi, 8
    add rdi, 8
    dec rcx
    jmp .loop_qword
.tail:
    and rdx, 7
    cmp rdx, 0
    je .ret
.loop_byte:
    mov al, BYTE PTR [rsi]
    mov BYTE PTR [rdi], al
    inc rsi
    inc rdi
    dec rdx
    jnz .loop_byte
.ret:
    ret
.att_syntax prefix
