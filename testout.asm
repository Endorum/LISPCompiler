
%include "stdlib.asm"
section .text
global _start
_start: 
    ;; init malloc
    mov eax, 8192
    mov [heap_start], eax ; size
    mov dword [heap_start+4], 0 ; next = 0
    mov dword [free_list], heap_start
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
main: ; 0 parameters 
    push ebp
    mov ebp, esp
    sub esp, 8
    mov dword [ebp - 4], 5
    mov dword [ebp - 8], 6
    mov ecx, [ebp - 4]
    mov [ebp], ecx
    mov edi, [ebp - 8]
    mov eax, [ebp]
    cmp eax, edi
    setg al
    and al, 1
    movzx eax, al
    mov esi, eax
    mov eax, esi
    cmp eax, 0
    je label_if_false_0
    jmp label_if_true_0
label_if_false_0:
    mov ecx, [ebp - 8]
    mov [t1], ecx
    mov eax, [t1]
    sub eax, [ebp - 4]
    mov [ebp - 4], eax
    mov ecx, [ebp - 4]
    mov [t0], ecx
    jmp label_if_end_0
label_if_true_0:
    mov ecx, [ebp - 8]
    mov [t2], ecx
    mov eax, [t2]
    add eax, [ebp - 4]
    mov [ebp - 4], eax
    mov ecx, [ebp - 4]
    mov [t0], ecx
label_if_end_0:
    mov eax, [t0]
    mov esp, ebp
    pop ebp
    ret
section .data
t0: dd 0
t1: dd 0
t2: dd 0
t3: dd 0
t4: dd 0
t5: dd 0
t6: dd 0
t7: dd 0
t8: dd 0
t9: dd 0
list_ptr: dd list_memory ; pointer to next free cell
heap_ptr: dd heap_start ; pointer to heap begin
free_list: dd heap_start
section .bss
list_memory: resb 8192; reserved (uninitialized!!) space for cons cells
heap_start: resb 8192; heap space

