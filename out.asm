
%include "stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
;; defun label, strlen_internal, 
strlen_internal:
    push ebp
    mov ebp, esp
    sub esp, 40
    ;; load t1, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 8], ecx
    ;; deref t2, t1, 
    mov eax, [ebp - 8]
    movzx ebx, byte [eax]
    mov [ebp - 12], ebx
    ;; eq t0, t2, 0
    mov eax, [ebp - 12]
    cmp eax, 0
    sete al
    movzx eax, al
    mov [ebp - 4], eax
    ;; if label_if_true_0, t0, label_if_false_0
    mov eax, [ebp - 4]
    cmp eax, 0
    je  label_if_false_0
    jmp label_if_true_0
    ;; label label_if_false_0, , 
label_if_false_0:
    ;; load t5, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 24], ecx
    ;; add t4, t5, 1
    mov eax, [ebp - 24]
    add eax, 1
    mov [ebp - 20], eax
    ;; load t7, n, 
    mov ecx, [ebp + 12]
    mov [ebp - 32], ecx
    ;; add t6, t7, 1
    mov eax, [ebp - 32]
    add eax, 1
    mov [ebp - 28], eax
    ;; push , t6, 
    push dword [ebp - 28]
    ;; push , t4, 
    push dword [ebp - 20]
    ;; call , strlen_internal, 
    call strlen_internal
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t8, , 
    mov [ebp - 36], eax
    ;; assign t3, t8, 
    mov ecx, [ebp - 36]
    mov [ebp - 16], ecx
    ;; jump label_if_end_0, , 
    jmp label_if_end_0
    ;; label label_if_true_0, , 
label_if_true_0:
    ;; load t9, n, 
    mov ecx, [ebp + 12]
    mov [ebp - 40], ecx
    ;; assign t3, t9, 
    mov ecx, [ebp - 40]
    mov [ebp - 16], ecx
    ;; label label_if_end_0, , 
label_if_end_0:
    ;; return return_value, t3, 
    mov eax, [ebp - 16]
    mov esp, ebp
    pop ebp
    ret
;; defun label, strlen, 
strlen:
    push ebp
    mov ebp, esp
    sub esp, 8
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; push , 0, 
    mov eax, 0
    push dword eax
    ;; push , t0, 
    push dword [ebp - 4]
    ;; call , strlen_internal, 
    call strlen_internal
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t1, , 
    mov [ebp - 8], eax
    ;; return return_value, t1, 
    mov eax, [ebp - 8]
    mov esp, ebp
    pop ebp
    ret
;; defun label, digitToString, 
digitToString:
    push ebp
    mov ebp, esp
    sub esp, 128
    ;; load t2, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 12], ecx
    ;; eq t1, t2, 0
    mov eax, [ebp - 12]
    cmp eax, 0
    sete al
    movzx eax, al
    mov [ebp - 8], eax
    ;; if label_clause_1_1, t1, label_next_1_1
    mov eax, [ebp - 8]
    cmp eax, 0
    je  label_next_1_1
    jmp label_clause_1_1
    ;; label label_clause_1_1, , 
label_clause_1_1:
    ;; add t3, 0, 48
    mov eax, 0
    add eax, 48
    mov [ebp - 16], eax
    ;; assign t0, t3, 
    mov ecx, [ebp - 16]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_1_1, , 
label_next_1_1:
    ;; load t5, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 24], ecx
    ;; eq t4, t5, 1
    mov eax, [ebp - 24]
    cmp eax, 1
    sete al
    movzx eax, al
    mov [ebp - 20], eax
    ;; if label_clause_2_1, t4, label_next_2_1
    mov eax, [ebp - 20]
    cmp eax, 0
    je  label_next_2_1
    jmp label_clause_2_1
    ;; label label_clause_2_1, , 
label_clause_2_1:
    ;; add t6, 1, 48
    mov eax, 1
    add eax, 48
    mov [ebp - 28], eax
    ;; assign t0, t6, 
    mov ecx, [ebp - 28]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_2_1, , 
label_next_2_1:
    ;; load t8, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 36], ecx
    ;; eq t7, t8, 2
    mov eax, [ebp - 36]
    cmp eax, 2
    sete al
    movzx eax, al
    mov [ebp - 32], eax
    ;; if label_clause_3_1, t7, label_next_3_1
    mov eax, [ebp - 32]
    cmp eax, 0
    je  label_next_3_1
    jmp label_clause_3_1
    ;; label label_clause_3_1, , 
label_clause_3_1:
    ;; add t9, 2, 48
    mov eax, 2
    add eax, 48
    mov [ebp - 40], eax
    ;; assign t0, t9, 
    mov ecx, [ebp - 40]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_3_1, , 
label_next_3_1:
    ;; load t11, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 48], ecx
    ;; eq t10, t11, 3
    mov eax, [ebp - 48]
    cmp eax, 3
    sete al
    movzx eax, al
    mov [ebp - 44], eax
    ;; if label_clause_4_1, t10, label_next_4_1
    mov eax, [ebp - 44]
    cmp eax, 0
    je  label_next_4_1
    jmp label_clause_4_1
    ;; label label_clause_4_1, , 
label_clause_4_1:
    ;; add t12, 3, 48
    mov eax, 3
    add eax, 48
    mov [ebp - 52], eax
    ;; assign t0, t12, 
    mov ecx, [ebp - 52]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_4_1, , 
label_next_4_1:
    ;; load t14, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 60], ecx
    ;; eq t13, t14, 4
    mov eax, [ebp - 60]
    cmp eax, 4
    sete al
    movzx eax, al
    mov [ebp - 56], eax
    ;; if label_clause_5_1, t13, label_next_5_1
    mov eax, [ebp - 56]
    cmp eax, 0
    je  label_next_5_1
    jmp label_clause_5_1
    ;; label label_clause_5_1, , 
label_clause_5_1:
    ;; add t15, 4, 48
    mov eax, 4
    add eax, 48
    mov [ebp - 64], eax
    ;; assign t0, t15, 
    mov ecx, [ebp - 64]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_5_1, , 
label_next_5_1:
    ;; load t17, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 72], ecx
    ;; eq t16, t17, 5
    mov eax, [ebp - 72]
    cmp eax, 5
    sete al
    movzx eax, al
    mov [ebp - 68], eax
    ;; if label_clause_6_1, t16, label_next_6_1
    mov eax, [ebp - 68]
    cmp eax, 0
    je  label_next_6_1
    jmp label_clause_6_1
    ;; label label_clause_6_1, , 
label_clause_6_1:
    ;; add t18, 5, 48
    mov eax, 5
    add eax, 48
    mov [ebp - 76], eax
    ;; assign t0, t18, 
    mov ecx, [ebp - 76]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_6_1, , 
label_next_6_1:
    ;; load t20, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 84], ecx
    ;; eq t19, t20, 6
    mov eax, [ebp - 84]
    cmp eax, 6
    sete al
    movzx eax, al
    mov [ebp - 80], eax
    ;; if label_clause_7_1, t19, label_next_7_1
    mov eax, [ebp - 80]
    cmp eax, 0
    je  label_next_7_1
    jmp label_clause_7_1
    ;; label label_clause_7_1, , 
label_clause_7_1:
    ;; add t21, 6, 48
    mov eax, 6
    add eax, 48
    mov [ebp - 88], eax
    ;; assign t0, t21, 
    mov ecx, [ebp - 88]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_7_1, , 
label_next_7_1:
    ;; load t23, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 96], ecx
    ;; eq t22, t23, 7
    mov eax, [ebp - 96]
    cmp eax, 7
    sete al
    movzx eax, al
    mov [ebp - 92], eax
    ;; if label_clause_8_1, t22, label_next_8_1
    mov eax, [ebp - 92]
    cmp eax, 0
    je  label_next_8_1
    jmp label_clause_8_1
    ;; label label_clause_8_1, , 
label_clause_8_1:
    ;; add t24, 7, 48
    mov eax, 7
    add eax, 48
    mov [ebp - 100], eax
    ;; assign t0, t24, 
    mov ecx, [ebp - 100]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_8_1, , 
label_next_8_1:
    ;; load t26, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 108], ecx
    ;; eq t25, t26, 8
    mov eax, [ebp - 108]
    cmp eax, 8
    sete al
    movzx eax, al
    mov [ebp - 104], eax
    ;; if label_clause_9_1, t25, label_next_9_1
    mov eax, [ebp - 104]
    cmp eax, 0
    je  label_next_9_1
    jmp label_clause_9_1
    ;; label label_clause_9_1, , 
label_clause_9_1:
    ;; add t27, 8, 48
    mov eax, 8
    add eax, 48
    mov [ebp - 112], eax
    ;; assign t0, t27, 
    mov ecx, [ebp - 112]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_9_1, , 
label_next_9_1:
    ;; load t29, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 120], ecx
    ;; eq t28, t29, 9
    mov eax, [ebp - 120]
    cmp eax, 9
    sete al
    movzx eax, al
    mov [ebp - 116], eax
    ;; if label_clause_10_1, t28, label_next_10_1
    mov eax, [ebp - 116]
    cmp eax, 0
    je  label_next_10_1
    jmp label_clause_10_1
    ;; label label_clause_10_1, , 
label_clause_10_1:
    ;; add t30, 9, 48
    mov eax, 9
    add eax, 48
    mov [ebp - 124], eax
    ;; assign t0, t30, 
    mov ecx, [ebp - 124]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_next_10_1, , 
label_next_10_1:
    ;; label label_clause_11_1, , 
label_clause_11_1:
    ;; loadstr t31, ERROR, 
    mov eax, str_0
    mov [ebp - 128], eax
    ;; assign t0, t31, 
    mov ecx, [ebp - 128]
    mov [ebp - 4], ecx
    ;; jump label_cond_end_1, , 
    jmp label_cond_end_1
    ;; label label_cond_end_1, , 
label_cond_end_1:
    ;; return return_value, t0, 
    mov eax, [ebp - 4]
    mov esp, ebp
    pop ebp
    ret
;; defun label, reverse_str, 
reverse_str:
    push ebp
    mov ebp, esp
    sub esp, 92
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; push , t0, 
    push dword [ebp - 4]
    ;; call , strlen, 
    call strlen
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t1, , 
    mov [ebp - 8], eax
    ;; assign len, t1, 
    mov ecx, [ebp - 8]
    mov [ebp - 12], ecx
    ;; loadstr t2, .........., 
    mov eax, str_1
    mov [ebp - 16], eax
    ;; assign out, t2, 
    mov ecx, [ebp - 16]
    mov [ebp - 20], ecx
    ;; assign i, 0, 
    mov dword [ebp - 24], 0
    ;; label label_while_start_2, , 
label_while_start_2:
    ;; load t4, i, 
    mov ecx, [ebp - 24]
    mov [ebp - 32], ecx
    ;; load t5, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 36], ecx
    ;; l t3, t4, t5
    mov eax, [ebp - 32]
    cmp eax, [ebp - 36]
    setl al
    movzx eax, al
    mov [ebp - 28], eax
    ;; if label_while_body_2, t3, label_while_end_2
    mov eax, [ebp - 28]
    cmp eax, 0
    je  label_while_end_2
    jmp label_while_body_2
    ;; label label_while_body_2, , 
label_while_body_2:
    ;; load t7, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 44], ecx
    ;; load t10, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 56], ecx
    ;; load t11, i, 
    mov ecx, [ebp - 24]
    mov [ebp - 60], ecx
    ;; sub t9, t10, t11
    mov eax, [ebp - 56]
    sub eax, [ebp - 60]
    mov [ebp - 52], eax
    ;; sub t8, t9, 1
    mov eax, [ebp - 52]
    sub eax, 1
    mov [ebp - 48], eax
    ;; add t6, t7, t8
    mov eax, [ebp - 44]
    add eax, [ebp - 48]
    mov [ebp - 40], eax
    ;; deref t12, t6, 
    mov eax, [ebp - 40]
    movzx ebx, byte [eax]
    mov [ebp - 64], ebx
    ;; assign c, t12, 
    mov ecx, [ebp - 64]
    mov [ebp - 68], ecx
    ;; load t13, out, 
    mov ecx, [ebp - 20]
    mov [ebp - 72], ecx
    ;; load t14, i, 
    mov ecx, [ebp - 24]
    mov [ebp - 76], ecx
    ;; load t15, c, 
    mov ecx, [ebp - 68]
    mov [ebp - 80], ecx
    ;; setchar t13, t14, t15
    mov eax, [ebp - 72]
    add eax, [ebp - 76]
    mov ecx, [ebp - 80]
    mov [eax], cl
    ;; load t17, i, 
    mov ecx, [ebp - 24]
    mov [ebp - 88], ecx
    ;; add t16, t17, 1
    mov eax, [ebp - 88]
    add eax, 1
    mov [ebp - 84], eax
    ;; assign i, t16, 
    mov ecx, [ebp - 84]
    mov [ebp - 24], ecx
    ;; jump label_while_start_2, , 
    jmp label_while_start_2
    ;; label label_while_end_2, , 
label_while_end_2:
    ;; load t18, out, 
    mov ecx, [ebp - 20]
    mov [ebp - 92], ecx
    ;; return return_value, t18, 
    mov eax, [ebp - 92]
    mov esp, ebp
    pop ebp
    ret
;; defun label, itoa, 
itoa:
    push ebp
    mov ebp, esp
    sub esp, 88
    ;; loadstr t0, .........., 
    mov eax, str_2
    mov [ebp - 4], eax
    ;; assign tmp, t0, 
    mov ecx, [ebp - 4]
    mov [ebp - 8], ecx
    ;; assign i, 0, 
    mov dword [ebp - 12], 0
    ;; label label_while_start_3, , 
label_while_start_3:
    ;; load t2, num, 
    mov ecx, [ebp + 8]
    mov [ebp - 20], ecx
    ;; g t1, t2, 0
    mov eax, [ebp - 20]
    cmp eax, 0
    setg al
    movzx eax, al
    mov [ebp - 16], eax
    ;; if label_while_body_3, t1, label_while_end_3
    mov eax, [ebp - 16]
    cmp eax, 0
    je  label_while_end_3
    jmp label_while_body_3
    ;; label label_while_body_3, , 
label_while_body_3:
    ;; load t4, num, 
    mov ecx, [ebp + 8]
    mov [ebp - 28], ecx
    ;; load t5, base, 
    mov ecx, [ebp + 12]
    mov [ebp - 32], ecx
    ;; mod t3, t4, t5
    mov eax, [ebp - 28]
    cdq
    mov ebx, dword [ebp - 32]
    idiv dword ebx
    mov [ebp - 24], edx
    ;; assign digit, t3, 
    mov ecx, [ebp - 24]
    mov [ebp - 36], ecx
    ;; load t6, digit, 
    mov ecx, [ebp - 36]
    mov [ebp - 40], ecx
    ;; push , t6, 
    push dword [ebp - 40]
    ;; call , digitToString, 
    call digitToString
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t7, , 
    mov [ebp - 44], eax
    ;; assign ch, t7, 
    mov ecx, [ebp - 44]
    mov [ebp - 48], ecx
    ;; load t8, tmp, 
    mov ecx, [ebp - 8]
    mov [ebp - 52], ecx
    ;; load t9, i, 
    mov ecx, [ebp - 12]
    mov [ebp - 56], ecx
    ;; load t10, ch, 
    mov ecx, [ebp - 48]
    mov [ebp - 60], ecx
    ;; setchar t8, t9, t10
    mov eax, [ebp - 52]
    add eax, [ebp - 56]
    mov ecx, [ebp - 60]
    mov [eax], cl
    ;; load t12, i, 
    mov ecx, [ebp - 12]
    mov [ebp - 68], ecx
    ;; addeq i, i, 1
    mov eax, 1
    mov ebx, [ebp - 12]
    add ebx, eax
    mov [ebp - 12], ebx
    ;; load t14, num, 
    mov ecx, [ebp + 8]
    mov [ebp - 76], ecx
    ;; load t15, base, 
    mov ecx, [ebp + 12]
    mov [ebp - 80], ecx
    ;; idiveq num, num, t15
    mov eax, [ebp + 8]
    cdq
    mov ebx, [ebp - 80]
    idiv ebx
    mov [ebp + 8], eax
    ;; jump label_while_start_3, , 
    jmp label_while_start_3
    ;; label label_while_end_3, , 
label_while_end_3:
    ;; load t16, tmp, 
    mov ecx, [ebp - 8]
    mov [ebp - 84], ecx
    ;; push , t16, 
    push dword [ebp - 84]
    ;; call , reverse_str, 
    call reverse_str
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t17, , 
    mov [ebp - 88], eax
    ;; return return_value, t17, 
    mov eax, [ebp - 88]
    mov esp, ebp
    pop ebp
    ret
;; defun label, main, 
main:
    push ebp
    mov ebp, esp
    sub esp, 4
    ;; push , 10, 
    mov eax, 10
    push dword eax
    ;; push , 123, 
    mov eax, 123
    push dword eax
    ;; call , itoa, 
    call itoa
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t0, , 
    mov [ebp - 4], eax
    ;; print stdout, t0, 
    mov eax, [ebp - 4]
    call _internal_print_string
    ;; return return_value, t0, 
    mov eax, [ebp - 4]
    mov esp, ebp
    pop ebp
    ret
section .data
str_0: db "ERROR", 0
str_1: db "..........", 0
str_2: db "..........", 0
list_ptr: dd list_memory ; pointer to next free cell
section .bss
list_memory: resb 1048576; reserved (uninitialized!!) space for cons cells

