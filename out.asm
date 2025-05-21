
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
main: ;0 parameters 
    push ebp
    mov ebp, esp
    sub esp, 20
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
    cmp eax, 0
    je label_if_false_0
    jmp label_if_true_0
label_if_false_0:
    mov ecx, [ebp - 8]
    mov [ebp - 16], ecx
    mov eax, [ebp - 16]
    sub eax, [ebp - 4]
    mov [ebp - 4], eax
    mov ecx, [ebp - 4]
    mov [ebp - 12], ecx
    jmp label_if_end_0
label_if_true_0:
    mov ecx, [ebp - 8]
    mov [ebp - 20], ecx
    mov eax, [ebp - 20]
    add eax, [ebp - 4]
    mov [ebp - 4], eax
    mov ecx, [ebp - 4]
    mov [ebp - 12], ecx
label_if_end_0:
    mov eax, [ebp - 12]
    mov esp, ebp
    pop ebp
    ret
section .data
list_ptr: dd list_memory ; pointer to next free cell
heap_ptr: dd heap_start ; pointer to heap begin
free_list: dd heap_start
section .bss
list_memory: resb 8192; reserved (uninitialized!!) space for cons cells
heap_start: resb 8192; heap space

