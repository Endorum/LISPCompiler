global print

section .text
_internal_print_string:
    ; Input: eax = pointer to null-terminated string

    push ebp
    mov ebp, esp

    push eax             ; save pointer to string
    call _internal_strlen ; get length in eax
    pop ebx              ; restore string pointer to ebx

    mov ecx, ebx         ; ecx = pointer to string
    mov edx, eax         ; edx = length
    mov ebx, 1           ; stdout (file descriptor)
    mov eax, 4           ; syscall: sys_write
    int 0x80

    pop ebp
    ret


_internal_strlen:
    ; Input: eax = pointer to string
    ; Output: eax = length

    push ebp
    mov ebp, esp

    push edi
    mov edi, eax
    xor eax, eax

.len_loop:
    cmp byte [edi], 0
    je .done
    inc edi
    inc eax
    jmp .len_loop

.done:
    pop edi
    pop ebp
    ret
