.section .data
msg: .ascii "Android Auditor\n"
len = . - msg

.section .text
.global _start
_start:
    mov $1, %rax        # sys_write
    mov $1, %rdi        # stdout
    lea msg(%rip), %rsi
    mov $len, %rdx
    syscall

    mov $60, %rax       # sys_exit
    xor %rdi, %rdi
    syscall
