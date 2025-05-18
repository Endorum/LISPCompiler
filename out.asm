
%include "stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
digitToString:
    push ebp
    mov ebp, esp
    sub esp, 128
    mov ecx, [ebp + 8]
    mov [ebp - 12], ecx
    mov eax, [ebp - 12]
    cmp eax, 0
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_1_0
    jmp label_next_1_0
label_clause_1_0:
    mov eax, str_0
    mov [ebp - 16], eax
    mov ecx, [ebp - 16]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_1_0:
    mov ecx, [ebp + 8]
    mov [ebp - 24], ecx
    mov eax, [ebp - 24]
    cmp eax, 1
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_2_0
    jmp label_next_2_0
label_clause_2_0:
    mov eax, str_1
    mov [ebp - 28], eax
    mov ecx, [ebp - 28]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_2_0:
    mov ecx, [ebp + 8]
    mov [ebp - 36], ecx
    mov eax, [ebp - 36]
    cmp eax, 2
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_3_0
    jmp label_next_3_0
label_clause_3_0:
    mov eax, str_2
    mov [ebp - 40], eax
    mov ecx, [ebp - 40]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_3_0:
    mov ecx, [ebp + 8]
    mov [ebp - 48], ecx
    mov eax, [ebp - 48]
    cmp eax, 3
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_4_0
    jmp label_next_4_0
label_clause_4_0:
    mov eax, str_3
    mov [ebp - 52], eax
    mov ecx, [ebp - 52]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_4_0:
    mov ecx, [ebp + 8]
    mov [ebp - 60], ecx
    mov eax, [ebp - 60]
    cmp eax, 4
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_5_0
    jmp label_next_5_0
label_clause_5_0:
    mov eax, str_4
    mov [ebp - 64], eax
    mov ecx, [ebp - 64]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_5_0:
    mov ecx, [ebp + 8]
    mov [ebp - 72], ecx
    mov eax, [ebp - 72]
    cmp eax, 5
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_6_0
    jmp label_next_6_0
label_clause_6_0:
    mov eax, str_5
    mov [ebp - 76], eax
    mov ecx, [ebp - 76]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_6_0:
    mov ecx, [ebp + 8]
    mov [ebp - 84], ecx
    mov eax, [ebp - 84]
    cmp eax, 6
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_7_0
    jmp label_next_7_0
label_clause_7_0:
    mov eax, str_6
    mov [ebp - 88], eax
    mov ecx, [ebp - 88]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_7_0:
    mov ecx, [ebp + 8]
    mov [ebp - 96], ecx
    mov eax, [ebp - 96]
    cmp eax, 7
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_8_0
    jmp label_next_8_0
label_clause_8_0:
    mov eax, str_7
    mov [ebp - 100], eax
    mov ecx, [ebp - 100]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_8_0:
    mov ecx, [ebp + 8]
    mov [ebp - 108], ecx
    mov eax, [ebp - 108]
    cmp eax, 8
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_9_0
    jmp label_next_9_0
label_clause_9_0:
    mov eax, str_8
    mov [ebp - 112], eax
    mov ecx, [ebp - 112]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_9_0:
    mov ecx, [ebp + 8]
    mov [ebp - 120], ecx
    mov eax, [ebp - 120]
    cmp eax, 9
    sete al
    movzx eax, al
    cmp eax, 0
    jne label_clause_10_0
    jmp label_next_10_0
label_clause_10_0:
    mov eax, str_9
    mov [ebp - 124], eax
    mov ecx, [ebp - 124]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_next_10_0:
label_clause_11_0:
    mov eax, str_10
    mov [ebp - 128], eax
    mov ecx, [ebp - 128]
    mov [ebp - 4], ecx
    jmp label_cond_end_0
label_cond_end_0:
    mov eax, [ebp - 4]
    mov esp, ebp
    pop ebp
    ret
main:
    push ebp
    mov ebp, esp
    sub esp, 4
    mov eax, 1234
    push dword eax
    call digitToString
    add esp, 4
    call _internal_print_string
    mov eax, [ebp - 4]
    mov esp, ebp
    pop ebp
    ret
section .data
str_0: db "0", 0
str_1: db "1", 0
str_2: db "2", 0
str_3: db "3", 0
str_4: db "4", 0
str_5: db "5", 0
str_6: db "6", 0
str_7: db "7", 0
str_8: db "8", 0
str_9: db "9", 0
str_10: db "ERROR", 0
list_ptr: dd list_memory ; pointer to next free cell
list_memory: times 1024 db 0 ; reserved space for cons cells

