section .text
global _main
_main:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov rax, 4
    mov [rbp - 0], rax
    mov rax, 5
    mov [rbp - 4], rax
    mov rax, [rbp - 0]
    imul rax, [rbp - 4]
    mov [rbp - 8], rax
    mov rax, 132
    mov [rbp - 12], rax
    mov rax, 43
    mov [rbp - 16], rax
    mov rax, [rbp - 12]
    sub rax, [rbp - 16]
    mov [rbp - 20], rax
    mov rax, [rbp - 8]
    add rax, [rbp - 20]
    mov [rbp - 24], rax
    mov rsp, rbp
    pop rbp
    mov rdi, [rbp - 24]
    mov rax, 0x2000001
    syscall

