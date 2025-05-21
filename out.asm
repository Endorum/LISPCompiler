
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
strlen_internal: ; 2 parameters 
    push ebp
    mov ebp, esp
    sub esp, 0
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    mov eax, [ebp]
    movzx ebx, byte [eax]
    mov esi, ebx
    mov eax, esi
    cmp eax, 0
    sete al
    and al, 1
    movzx eax, al
    mov edi, eax
    mov eax, edi
    cmp eax, 0
    je label_if_false_0
    jmp label_if_true_0
label_if_false_0:
    mov ecx, [ebp + 8]
    mov [t1], ecx
    mov eax, [t1]
    add eax, 1
    mov [t2], eax
    mov ecx, [ebp + 12]
    mov [t3], ecx
    mov eax, [t3]
    add eax, 1
    mov [t4], eax
    push dword [t4]
    push dword [t2]
    call strlen_internal
    add esp, 8
    mov [t5], eax
    mov ecx, [t5]
    mov [t0], ecx
    jmp label_if_end_0
label_if_true_0:
    mov ecx, [ebp + 12]
    mov [t6], ecx
    mov ecx, [t6]
    mov [t0], ecx
label_if_end_0:
    mov eax, [t0]
    mov esp, ebp
    pop ebp
    ret
strlen: ; 1 parameters 
    push ebp
    mov ebp, esp
    sub esp, 0
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    mov eax, 0
    push dword eax
    push dword [ebp]
    call strlen_internal
    add esp, 8
    mov esi, eax
    mov eax, esi
    mov esp, ebp
    pop ebp
    ret
digitToString: ; 1 parameters 
    push ebp
    mov ebp, esp
    sub esp, 0
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    mov eax, [ebp]
    cmp eax, 10
    setl al
    and al, 1
    movzx eax, al
    mov esi, eax
    mov eax, esi
    cmp eax, 0
    je label_if_false_1
    jmp label_if_true_1
label_if_false_1:
    mov ecx, [ebp + 8]
    mov [t0], ecx
    mov eax, [t0]
    add eax, 55
    mov [t1], eax
    mov edi, [t1]
    jmp label_if_end_1
label_if_true_1:
    mov ecx, [ebp + 8]
    mov [t2], ecx
    mov eax, [t2]
    add eax, 48
    mov [t3], eax
    mov edi, [t3]
label_if_end_1:
    mov eax, edi
    mov esp, ebp
    pop ebp
    ret
reverse_str: ; 1 parameters 
    push ebp
    mov ebp, esp
    sub esp, 40
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    push dword [ebp]
    call strlen
    add esp, 4
    mov esi, eax
    mov [ebp - 4], esi
    mov eax, str_0
    mov edi, eax
    mov [ebp - 8], edi
    mov dword [ebp - 12], 0
label_while_start_2:
    mov ecx, [ebp - 12]
    mov [t0], ecx
    mov ecx, [ebp - 4]
    mov [t2], ecx
    mov eax, [t0]
    cmp eax, [t2]
    setl al
    and al, 1
    movzx eax, al
    mov [t1], eax
    mov eax, [t1]
    cmp eax, 0
    je label_while_end_2
    jmp label_while_body_2
label_while_body_2:
    mov ecx, [ebp + 8]
    mov [t3], ecx
    mov ecx, [ebp - 4]
    mov [t5], ecx
    mov ecx, [ebp - 12]
    mov [t7], ecx
    mov eax, [t5]
    sub eax, [t7]
    mov [t6], eax
    mov eax, [t6]
    sub eax, 1
    mov [t8], eax
    mov eax, [t3]
    add eax, [t8]
    mov [t4], eax
    mov eax, [t4]
    movzx ebx, byte [eax]
    mov [t9], ebx
    mov ecx, [t9]
    mov [ebp - 16], ecx
    mov ecx, [ebp - 8]
    mov [ebp - 20], ecx
    mov ecx, [ebp - 12]
    mov [ebp - 24], ecx
    mov ecx, [ebp - 16]
    mov [ebp - 28], ecx
    mov eax, [ebp - 20]
    add eax, [ebp - 24]
    mov ecx, [ebp - 28]
    mov [eax], cl
    mov ecx, [ebp - 12]
    mov [ebp - 32], ecx
    mov eax, [ebp - 32]
    add eax, 1
    mov [ebp - 36], eax
    mov ecx, [ebp - 36]
    mov [ebp - 12], ecx
    jmp label_while_start_2
label_while_end_2:
    mov ecx, [ebp - 8]
    mov [ebp - 40], ecx
    mov eax, [ebp - 40]
    mov esp, ebp
    pop ebp
    ret
printLn: ; 1 parameters 
    push ebp
    mov ebp, esp
    sub esp, 0
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    mov eax, [ebp]
    call _internal_print_string
    mov eax, str_1
    mov esi, eax
    mov eax, esi
    call _internal_print_string
    mov eax, esi
    mov esp, ebp
    pop ebp
    ret
itoa: ; 2 parameters 
    push ebp
    mov ebp, esp
    sub esp, 20
    mov eax, str_2
    mov [ebp], eax
    mov ecx, [ebp]
    mov [ebp - 4], ecx
    mov dword [ebp - 8], 0
label_while_start_3:
    mov esi, [ebp + 8]
    mov eax, esi
    cmp eax, 0
    setg al
    and al, 1
    movzx eax, al
    mov edi, eax
    mov eax, edi
    cmp eax, 0
    je label_while_end_3
    jmp label_while_body_3
label_while_body_3:
    mov ecx, [ebp + 8]
    mov [t0], ecx
    mov ecx, [ebp + 12]
    mov [t2], ecx
    mov eax, [t0]
    cdq
    idiv dword [t2]
    mov [t1], eax
    mov ecx, [t1]
    mov [ebp - 12], ecx
    mov ecx, [ebp - 12]
    mov [t3], ecx
    push dword [t3]
    call digitToString
    add esp, 4
    mov [t4], eax
    mov ecx, [t4]
    mov [ebp - 16], ecx
    mov ecx, [ebp - 4]
    mov [t5], ecx
    mov ecx, [ebp - 8]
    mov [t6], ecx
    mov ecx, [ebp - 16]
    mov [t7], ecx
    mov eax, [t5]
    add eax, [t6]
    mov ecx, [t7]
    mov [eax], cl
    mov eax, 1
    add eax, [ebp - 8]
    mov [ebp - 8], eax
    mov ecx, [ebp + 12]
    mov [t8], ecx
    mov ebx, [ebp + 8]
    mov eax, [t8]
    cdq
    idiv dword ebx
    mov [ebp + 8], eax
    jmp label_while_start_3
label_while_end_3:
    mov ecx, [ebp - 4]
    mov [t9], ecx
    push dword [t9]
    call reverse_str
    add esp, 4
    mov [ebp - 20], eax
    mov eax, [ebp - 20]
    mov esp, ebp
    pop ebp
    ret
readchar: ; 2 parameters 
    push ebp
    mov ebp, esp
    sub esp, 0
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    mov edi, [ebp + 12]
    mov eax, [ebp]
    add eax, edi
    mov esi, eax
    mov eax, esi
    movzx ebx, byte [eax]
    mov [t0], ebx
    mov eax, [t0]
    mov esp, ebp
    pop ebp
    ret
memcpy: ; 3 parameters 
    push ebp
    mov ebp, esp
    sub esp, 8
    mov dword [ebp - 4], 0
label_while_start_4:
    mov ecx, [ebp - 4]
    mov [ebp], ecx
    mov edi, [ebp + 16]
    mov eax, [ebp]
    cmp eax, edi
    setl al
    and al, 1
    movzx eax, al
    mov esi, eax
    mov eax, esi
    cmp eax, 0
    je label_while_end_4
    jmp label_while_body_4
label_while_body_4:
    mov ecx, [ebp + 12]
    mov [t0], ecx
    mov ecx, [ebp - 4]
    mov [t1], ecx
    push dword [t1]
    push dword [t0]
    call readchar
    add esp, 8
    mov [t2], eax
    mov ecx, [t2]
    mov [ebp - 8], ecx
    mov ecx, [ebp + 8]
    mov [t3], ecx
    mov ecx, [ebp - 4]
    mov [t4], ecx
    mov ecx, [ebp - 8]
    mov [t5], ecx
    mov eax, [t3]
    add eax, [t4]
    mov ecx, [t5]
    mov [eax], cl
    mov eax, 1
    add eax, [ebp - 4]
    mov [ebp - 4], eax
    jmp label_while_start_4
label_while_end_4:
    mov ecx, [ebp + 8]
    mov [t6], ecx
    mov eax, [t6]
    mov esp, ebp
    pop ebp
    ret
strcat: ; 2 parameters 
    push ebp
    mov ebp, esp
    sub esp, 56
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    push dword [ebp]
    call strlen
    add esp, 4
    mov esi, eax
    mov [ebp - 4], esi
    mov edi, [ebp + 12]
    push dword edi
    call strlen
    add esp, 4
    mov [t0], eax
    mov ecx, [t0]
    mov [ebp - 8], ecx
    mov ecx, [ebp - 4]
    mov [t1], ecx
    mov ecx, [ebp - 8]
    mov [t3], ecx
    mov eax, [t1]
    add eax, [t3]
    mov [t2], eax
    mov ecx, [t2]
    mov [ebp - 12], ecx
    mov ecx, [ebp - 12]
    mov [t4], ecx
    mov eax, [t4]
    add eax, 1
    mov [t5], eax
    mov eax, [t5]; block size
    call malloc ; calling internal malloc
    mov [t6], eax ; output pointer in eax
    mov ecx, [t6]
    mov [ebp - 16], ecx
    mov ecx, [ebp - 16]
    mov [t7], ecx
    mov ecx, [ebp + 8]
    mov [t8], ecx
    mov ecx, [ebp - 4]
    mov [t9], ecx
    push dword [t9]
    push dword [t8]
    push dword [t7]
    call memcpy
    add esp, 12
    mov [ebp - 20], eax
    mov ecx, [ebp - 16]
    mov [ebp - 24], ecx
    mov ecx, [ebp - 4]
    mov [ebp - 32], ecx
    mov eax, [ebp - 24]
    add eax, [ebp - 32]
    mov [ebp - 28], eax
    mov ecx, [ebp + 12]
    mov [ebp - 36], ecx
    mov ecx, [ebp - 8]
    mov [ebp - 40], ecx
    push dword [ebp - 40]
    push dword [ebp - 36]
    push dword [ebp - 28]
    call memcpy
    add esp, 12
    mov [ebp - 44], eax
    mov ecx, [ebp - 16]
    mov [ebp - 48], ecx
    mov ecx, [ebp - 12]
    mov [ebp - 52], ecx
    mov eax, [ebp - 48]
    add eax, [ebp - 52]
    mov ecx, 0
    mov [eax], cl
    mov ecx, [ebp - 16]
    mov [ebp - 56], ecx
    mov eax, [ebp - 56]
    mov esp, ebp
    pop ebp
    ret
alloc_string: ; 1 parameters 
    push ebp
    mov ebp, esp
    sub esp, 8
    mov ecx, [ebp + 8]
    mov [ebp], ecx
    push dword [ebp]
    call strlen
    add esp, 4
    mov esi, eax
    mov [ebp - 4], esi
    mov edi, [ebp - 4]
    mov eax, edi
    add eax, 1
    mov [t0], eax
    mov eax, [t0]; block size
    call malloc ; calling internal malloc
    mov [t1], eax ; output pointer in eax
    mov ecx, [t1]
    mov [ebp - 8], ecx
    mov ecx, [ebp - 8]
    mov [t2], ecx
    mov ecx, [ebp + 8]
    mov [t3], ecx
    mov ecx, [ebp - 4]
    mov [t4], ecx
    push dword [t4]
    push dword [t3]
    push dword [t2]
    call memcpy
    add esp, 12
    mov [t5], eax
    mov ecx, [ebp - 8]
    mov [t6], ecx
    mov ecx, [ebp - 4]
    mov [t7], ecx
    mov eax, [t6]
    add eax, [t7]
    mov ecx, 0
    mov [eax], cl
    mov ecx, [ebp - 8]
    mov [t8], ecx
    mov eax, [t8]
    mov esp, ebp
    pop ebp
    ret
main: ; 0 parameters 
    push ebp
    mov ebp, esp
    sub esp, 0
    mov eax, 10
    push dword eax
    mov eax, 123
    push dword eax
    call itoa
    add esp, 8
    mov [ebp], eax
    push dword [ebp]
    call printLn
    add esp, 4
    mov esi, eax
    mov eax, esi
    mov esp, ebp
    pop ebp
    ret
section .data
str_0: db "..........", 0
str_1: db "", 10, "", 0
str_2: db "..........", 0
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

