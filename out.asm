
%include "../stdio.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
; label defun main
main:
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 4
    ; t0 loadstr this is a string\n
    mov eax, str_0
    mov [ebp - 4], eax
    ; stdout print t0
    mov eax, [ebp - 4]
    call _internal_print_string    ; return_value return t0
    mov ebx, [ebp - 4]
    mov eax, ebx
    mov [ebp + 8], eax
    ; Stack frame cleanup
    mov esp, ebp
    pop ebp
    ret


section .data
str_0: db "this is a string\n", 0
