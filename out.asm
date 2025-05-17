
%include "stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
add:
    push ebp
    mov ebp, esp
    sub esp, 12
    mov ecx, [ebp + 8]
    mov [ebp - 8], ecx
    mov ecx, [ebp + 12]
    mov [ebp - 12], ecx
    mov eax, [ebp - 8]
    add eax, [ebp - 12]
    mov esp, ebp
    pop ebp
    ret
main:
    push ebp
    mov ebp, esp
    sub esp, 12
    mov eax, 2
    push dword eax
    mov eax, 1
    push dword eax
    call add
    add esp, 8
    mov [ebp - 4], eax
    mov eax, 4
    push dword eax
    mov eax, 3
    push dword eax
    call add
    add esp, 8
    mov [ebp - 8], eax
    push dword [ebp - 8]
    push dword [ebp - 4]
    call add
    add esp, 8
    mov esp, ebp
    pop ebp
    ret

section .data
return_address: dd main
list_memory: times 1024 db 0 ; reserved space for cons cells
list_ptr: dd list_memory ; pointer to next free cell
