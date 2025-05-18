
%include "stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
main:
    push ebp
    mov ebp, esp
    sub esp, 12
    ; load car (src1) into eax
    mov eax, 3
    ; store car into [list_ptr]
    mov ebx, [list_ptr]
    mov [ebx], eax
    ; load cdr (src2) into eax
    mov eax, 0
    ; Load cdr into [list_ptr + 4]
    mov ebx, [list_ptr]
    mov [ebx + 4], eax
    ; Store address of cons cell into dst
    mov eax, [list_ptr]
    mov [ebp - 4], eax
    ; Increment list pointer by 8 for next allocation
    add dword [list_ptr], 8
    ; load car (src1) into eax
    mov eax, 2
    ; store car into [list_ptr]
    mov ebx, [list_ptr]
    mov [ebx], eax
    ; load cdr (src2) into eax
    mov eax, [ebp - 4]
    ; Load cdr into [list_ptr + 4]
    mov ebx, [list_ptr]
    mov [ebx + 4], eax
    ; Store address of cons cell into dst
    mov eax, [list_ptr]
    mov [ebp - 8], eax
    ; Increment list pointer by 8 for next allocation
    add dword [list_ptr], 8
    ; load car (src1) into eax
    mov eax, 1
    ; store car into [list_ptr]
    mov ebx, [list_ptr]
    mov [ebx], eax
    ; load cdr (src2) into eax
    mov eax, [ebp - 8]
    ; Load cdr into [list_ptr + 4]
    mov ebx, [list_ptr]
    mov [ebx + 4], eax
    ; Store address of cons cell into dst
    mov eax, [list_ptr]
    mov [ebp - 12], eax
    ; Increment list pointer by 8 for next allocation
    add dword [list_ptr], 8
    mov eax, [ebp - 12]
    mov esp, ebp
    pop ebp
    ret
section .data
list_ptr: dd list_memory ; pointer to next free cell
section .bss
list_memory: resb 1048576; reserved (uninitialized!!) space for cons cells

