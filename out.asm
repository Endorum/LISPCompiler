
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
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; deref t1, t0, 
    mov eax, [ebp - 4]
    movzx ebx, byte [eax]
    mov [ebp - 8], ebx
    ;; eq t2, t1, 0
    mov eax, [ebp - 8]
    cmp eax, 0
    sete al
    movzx eax, al
    mov [ebp - 12], eax
    ;; if label_if_true_0, t2, label_if_false_0
    mov eax, [ebp - 12]
    cmp eax, 0
    je  label_if_false_0
    jmp label_if_true_0
    ;; label label_if_false_0, , 
label_if_false_0:
    ;; load t4, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 20], ecx
    ;; add t5, t4, 1
    mov eax, [ebp - 20]
    add eax, 1
    mov [ebp - 24], eax
    ;; load t6, n, 
    mov ecx, [ebp + 12]
    mov [ebp - 28], ecx
    ;; add t7, t6, 1
    mov eax, [ebp - 28]
    add eax, 1
    mov [ebp - 32], eax
    ;; push , t7, 
    push dword [ebp - 32]
    ;; push , t5, 
    push dword [ebp - 24]
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
    sub esp, 28
    ;; load t0, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; l t1, t0, 10
    mov eax, [ebp - 4]
    cmp eax, 10
    setl al
    movzx eax, al
    mov [ebp - 8], eax
    ;; if label_if_true_1, t1, label_if_false_1
    mov eax, [ebp - 8]
    cmp eax, 0
    je  label_if_false_1
    jmp label_if_true_1
    ;; label label_if_false_1, , 
label_if_false_1:
    ;; load t3, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 16], ecx
    ;; add t4, t3, 55
    mov eax, [ebp - 16]
    add eax, 55
    mov [ebp - 20], eax
    ;; assign t2, t4, 
    mov ecx, [ebp - 20]
    mov [ebp - 12], ecx
    ;; jump label_if_end_1, , 
    jmp label_if_end_1
    ;; label label_if_true_1, , 
label_if_true_1:
    ;; load t5, x, 
    mov ecx, [ebp + 8]
    mov [ebp - 24], ecx
    ;; add t6, t5, 48
    mov eax, [ebp - 24]
    add eax, 48
    mov [ebp - 28], eax
    ;; assign t2, t6, 
    mov ecx, [ebp - 28]
    mov [ebp - 12], ecx
    ;; label label_if_end_1, , 
label_if_end_1:
    ;; return return_value, t2, 
    mov eax, [ebp - 12]
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
    mov eax, str_0
    mov [ebp - 16], eax
    ;; assign out, t2, 
    mov ecx, [ebp - 16]
    mov [ebp - 20], ecx
    ;; assign i, 0, 
    mov dword [ebp - 24], 0
    ;; label label_while_start_2, , 
label_while_start_2:
    ;; load t3, i, 
    mov ecx, [ebp - 24]
    mov [ebp - 28], ecx
    ;; load t4, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 32], ecx
    ;; l t5, t3, t4
    mov eax, [ebp - 28]
    cmp eax, [ebp - 32]
    setl al
    movzx eax, al
    mov [ebp - 36], eax
    ;; if label_while_body_2, t5, label_while_end_2
    mov eax, [ebp - 36]
    cmp eax, 0
    je  label_while_end_2
    jmp label_while_body_2
    ;; label label_while_body_2, , 
label_while_body_2:
    ;; load t6, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 40], ecx
    ;; load t7, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 44], ecx
    ;; load t8, i, 
    mov ecx, [ebp - 24]
    mov [ebp - 48], ecx
    ;; sub t9, t7, t8
    mov eax, [ebp - 44]
    sub eax, [ebp - 48]
    mov [ebp - 52], eax
    ;; sub t10, t9, 1
    mov eax, [ebp - 52]
    sub eax, 1
    mov [ebp - 56], eax
    ;; add t11, t6, t10
    mov eax, [ebp - 40]
    add eax, [ebp - 56]
    mov [ebp - 60], eax
    ;; deref t12, t11, 
    mov eax, [ebp - 60]
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
    ;; load t16, i, 
    mov ecx, [ebp - 24]
    mov [ebp - 84], ecx
    ;; add t17, t16, 1
    mov eax, [ebp - 84]
    add eax, 1
    mov [ebp - 88], eax
    ;; assign i, t17, 
    mov ecx, [ebp - 88]
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
;; defun label, printLn, 
printLn:
    push ebp
    mov ebp, esp
    sub esp, 8
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; print stdout, t0, 
    mov eax, [ebp - 4]
    call _internal_print_string
    ;; loadstr t1, \n, 
    mov eax, str_1
    mov [ebp - 8], eax
    ;; print stdout, t1, 
    mov eax, [ebp - 8]
    call _internal_print_string
    ;; return return_value, t1, 
    mov eax, [ebp - 8]
    mov esp, ebp
    pop ebp
    ret
;; defun label, itoa, 
itoa:
    push ebp
    mov ebp, esp
    sub esp, 72
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
    ;; load t1, num, 
    mov ecx, [ebp + 8]
    mov [ebp - 16], ecx
    ;; g t2, t1, 0
    mov eax, [ebp - 16]
    cmp eax, 0
    setg al
    movzx eax, al
    mov [ebp - 20], eax
    ;; if label_while_body_3, t2, label_while_end_3
    mov eax, [ebp - 20]
    cmp eax, 0
    je  label_while_end_3
    jmp label_while_body_3
    ;; label label_while_body_3, , 
label_while_body_3:
    ;; load t3, num, 
    mov ecx, [ebp + 8]
    mov [ebp - 24], ecx
    ;; load t4, base, 
    mov ecx, [ebp + 12]
    mov [ebp - 28], ecx
    ;; mod t5, t3, t4
    mov eax, [ebp - 24]
    cdq
    mov ebx, dword [ebp - 28]
    idiv dword ebx
    mov [ebp - 32], edx
    ;; assign digit, t5, 
    mov ecx, [ebp - 32]
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
    ;; addeq i, i, 1
    mov eax, 1
    mov ebx, [ebp - 12]
    add ebx, eax
    mov [ebp - 12], ebx
    ;; load t11, base, 
    mov ecx, [ebp + 12]
    mov [ebp - 64], ecx
    ;; idiveq num, num, t11
    mov eax, [ebp + 8]
    cdq
    mov ebx, [ebp - 64]
    idiv ebx
    mov [ebp + 8], eax
    ;; jump label_while_start_3, , 
    jmp label_while_start_3
    ;; label label_while_end_3, , 
label_while_end_3:
    ;; load t12, tmp, 
    mov ecx, [ebp - 8]
    mov [ebp - 68], ecx
    ;; push , t12, 
    push dword [ebp - 68]
    ;; call , reverse_str, 
    call reverse_str
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t13, , 
    mov [ebp - 72], eax
    ;; return return_value, t13, 
    mov eax, [ebp - 72]
    mov esp, ebp
    pop ebp
    ret
;; defun label, readchar, 
readchar:
    push ebp
    mov ebp, esp
    sub esp, 16
    ;; load t0, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; load t1, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 8], ecx
    ;; add t2, t0, t1
    mov eax, [ebp - 4]
    add eax, [ebp - 8]
    mov [ebp - 12], eax
    ;; deref t3, t2, 
    mov eax, [ebp - 12]
    movzx ebx, byte [eax]
    mov [ebp - 16], ebx
    ;; return return_value, t3, 
    mov eax, [ebp - 16]
    mov esp, ebp
    pop ebp
    ret
;; defun label, memcpy, 
memcpy:
    push ebp
    mov ebp, esp
    sub esp, 48
    ;; assign i, 0, 
    mov dword [ebp - 4], 0
    ;; label label_while_start_4, , 
label_while_start_4:
    ;; load t0, i, 
    mov ecx, [ebp - 4]
    mov [ebp - 8], ecx
    ;; load t1, num, 
    mov ecx, [ebp + 16]
    mov [ebp - 12], ecx
    ;; l t2, t0, t1
    mov eax, [ebp - 8]
    cmp eax, [ebp - 12]
    setl al
    movzx eax, al
    mov [ebp - 16], eax
    ;; if label_while_body_4, t2, label_while_end_4
    mov eax, [ebp - 16]
    cmp eax, 0
    je  label_while_end_4
    jmp label_while_body_4
    ;; label label_while_body_4, , 
label_while_body_4:
    ;; load t3, ptrSrc, 
    mov ecx, [ebp + 12]
    mov [ebp - 20], ecx
    ;; load t4, i, 
    mov ecx, [ebp - 4]
    mov [ebp - 24], ecx
    ;; push , t4, 
    push dword [ebp - 24]
    ;; push , t3, 
    push dword [ebp - 20]
    ;; call , readchar, 
    call readchar
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t5, , 
    mov [ebp - 28], eax
    ;; assign ch, t5, 
    mov ecx, [ebp - 28]
    mov [ebp - 32], ecx
    ;; load t6, ptrDst, 
    mov ecx, [ebp + 8]
    mov [ebp - 36], ecx
    ;; load t7, i, 
    mov ecx, [ebp - 4]
    mov [ebp - 40], ecx
    ;; load t8, ch, 
    mov ecx, [ebp - 32]
    mov [ebp - 44], ecx
    ;; setchar t6, t7, t8
    mov eax, [ebp - 36]
    add eax, [ebp - 40]
    mov ecx, [ebp - 44]
    mov [eax], cl
    ;; addeq i, i, 1
    mov eax, 1
    mov ebx, [ebp - 4]
    add ebx, eax
    mov [ebp - 4], ebx
    ;; jump label_while_start_4, , 
    jmp label_while_start_4
    ;; label label_while_end_4, , 
label_while_end_4:
    ;; load t9, ptrDst, 
    mov ecx, [ebp + 8]
    mov [ebp - 48], ecx
    ;; return return_value, t9, 
    mov eax, [ebp - 48]
    mov esp, ebp
    pop ebp
    ret
;; defun label, string, 
string:
    push ebp
    mov ebp, esp
    sub esp, 56
    ;; load t0, init, 
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
    ;; load t2, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 16], ecx
    ;; add t3, t2, 1
    mov eax, [ebp - 16]
    add eax, 1
    mov [ebp - 20], eax
    ;; malloc t4, t3, 
    mov eax, [heap_ptr]
    mov [ebp - 24], eax
    mov ecx, [ebp - 20]
    add eax, ecx
    mov [heap_ptr], eax
    ;; assign str, t4, 
    mov ecx, [ebp - 24]
    mov [ebp - 28], ecx
    ;; load t5, str, 
    mov ecx, [ebp - 28]
    mov [ebp - 32], ecx
    ;; load t6, init, 
    mov ecx, [ebp + 8]
    mov [ebp - 36], ecx
    ;; load t7, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 40], ecx
    ;; push , t7, 
    push dword [ebp - 40]
    ;; push , t6, 
    push dword [ebp - 36]
    ;; push , t5, 
    push dword [ebp - 32]
    ;; call , memcpy, 
    call memcpy
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t8, , 
    mov [ebp - 44], eax
    ;; load t9, str, 
    mov ecx, [ebp - 28]
    mov [ebp - 48], ecx
    ;; load t10, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 52], ecx
    ;; setchar t9, t10, 0
    mov eax, [ebp - 48]
    add eax, [ebp - 52]
    mov ecx, 0
    mov [eax], cl
    ;; load t11, str, 
    mov ecx, [ebp - 28]
    mov [ebp - 56], ecx
    ;; return return_value, t11, 
    mov eax, [ebp - 56]
    mov esp, ebp
    pop ebp
    ret
;; defun label, strcat, 
strcat:
    push ebp
    mov ebp, esp
    sub esp, 108
    ;; load t0, strA, 
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
    ;; assign lenA, t1, 
    mov ecx, [ebp - 8]
    mov [ebp - 12], ecx
    ;; load t2, strB, 
    mov ecx, [ebp + 12]
    mov [ebp - 16], ecx
    ;; push , t2, 
    push dword [ebp - 16]
    ;; call , strlen, 
    call strlen
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t3, , 
    mov [ebp - 20], eax
    ;; assign lenB, t3, 
    mov ecx, [ebp - 20]
    mov [ebp - 24], ecx
    ;; load t4, lenA, 
    mov ecx, [ebp - 12]
    mov [ebp - 28], ecx
    ;; load t5, lenB, 
    mov ecx, [ebp - 24]
    mov [ebp - 32], ecx
    ;; add t6, t4, t5
    mov eax, [ebp - 28]
    add eax, [ebp - 32]
    mov [ebp - 36], eax
    ;; assign res_len, t6, 
    mov ecx, [ebp - 36]
    mov [ebp - 40], ecx
    ;; load t7, res_len, 
    mov ecx, [ebp - 40]
    mov [ebp - 44], ecx
    ;; add t8, t7, 1
    mov eax, [ebp - 44]
    add eax, 1
    mov [ebp - 48], eax
    ;; malloc t9, t8, 
    mov eax, [heap_ptr]
    mov [ebp - 52], eax
    mov ecx, [ebp - 48]
    add eax, ecx
    mov [heap_ptr], eax
    ;; assign res_str, t9, 
    mov ecx, [ebp - 52]
    mov [ebp - 56], ecx
    ;; load t10, res_str, 
    mov ecx, [ebp - 56]
    mov [ebp - 60], ecx
    ;; load t11, strA, 
    mov ecx, [ebp + 8]
    mov [ebp - 64], ecx
    ;; load t12, lenA, 
    mov ecx, [ebp - 12]
    mov [ebp - 68], ecx
    ;; push , t12, 
    push dword [ebp - 68]
    ;; push , t11, 
    push dword [ebp - 64]
    ;; push , t10, 
    push dword [ebp - 60]
    ;; call , memcpy, 
    call memcpy
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t13, , 
    mov [ebp - 72], eax
    ;; load t14, res_str, 
    mov ecx, [ebp - 56]
    mov [ebp - 76], ecx
    ;; load t15, lenA, 
    mov ecx, [ebp - 12]
    mov [ebp - 80], ecx
    ;; add t16, t14, t15
    mov eax, [ebp - 76]
    add eax, [ebp - 80]
    mov [ebp - 84], eax
    ;; load t17, strB, 
    mov ecx, [ebp + 12]
    mov [ebp - 88], ecx
    ;; load t18, lenB, 
    mov ecx, [ebp - 24]
    mov [ebp - 92], ecx
    ;; push , t18, 
    push dword [ebp - 92]
    ;; push , t17, 
    push dword [ebp - 88]
    ;; push , t16, 
    push dword [ebp - 84]
    ;; call , memcpy, 
    call memcpy
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t19, , 
    mov [ebp - 96], eax
    ;; load t20, res_str, 
    mov ecx, [ebp - 56]
    mov [ebp - 100], ecx
    ;; load t21, res_len, 
    mov ecx, [ebp - 40]
    mov [ebp - 104], ecx
    ;; setchar t20, t21, 0
    mov eax, [ebp - 100]
    add eax, [ebp - 104]
    mov ecx, 0
    mov [eax], cl
    ;; load t22, res_str, 
    mov ecx, [ebp - 56]
    mov [ebp - 108], ecx
    ;; return return_value, t22, 
    mov eax, [ebp - 108]
    mov esp, ebp
    pop ebp
    ret
;; defun label, setint, 
setint:
    push ebp
    mov ebp, esp
    sub esp, 96
    ;; load t0, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; load t1, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 8], ecx
    ;; add t2, 0, t1
    mov eax, 0
    add eax, [ebp - 8]
    mov [ebp - 12], eax
    ;; load t3, value, 
    mov ecx, [ebp + 16]
    mov [ebp - 16], ecx
    ;; and t4, t3, 255
    mov eax, [ebp - 16]
    and eax, 255
    mov [ebp - 20], eax
    ;; setchar t0, t2, t4
    mov eax, [ebp - 4]
    add eax, [ebp - 12]
    mov ecx, [ebp - 20]
    mov [eax], cl
    ;; load t5, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 24], ecx
    ;; load t6, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 28], ecx
    ;; add t7, 1, t6
    mov eax, 1
    add eax, [ebp - 28]
    mov [ebp - 32], eax
    ;; load t8, value, 
    mov ecx, [ebp + 16]
    mov [ebp - 36], ecx
    ;; shr t9, t8, 8
    mov eax, [ebp - 36]
    mov cl, 8
    shr eax, cl
    mov [ebp - 40], eax
    ;; and t10, t9, 255
    mov eax, [ebp - 40]
    and eax, 255
    mov [ebp - 44], eax
    ;; setchar t5, t7, t10
    mov eax, [ebp - 24]
    add eax, [ebp - 32]
    mov ecx, [ebp - 44]
    mov [eax], cl
    ;; load t11, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 48], ecx
    ;; load t12, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 52], ecx
    ;; add t13, 2, t12
    mov eax, 2
    add eax, [ebp - 52]
    mov [ebp - 56], eax
    ;; load t14, value, 
    mov ecx, [ebp + 16]
    mov [ebp - 60], ecx
    ;; shr t15, t14, 16
    mov eax, [ebp - 60]
    mov cl, 16
    shr eax, cl
    mov [ebp - 64], eax
    ;; and t16, t15, 255
    mov eax, [ebp - 64]
    and eax, 255
    mov [ebp - 68], eax
    ;; setchar t11, t13, t16
    mov eax, [ebp - 48]
    add eax, [ebp - 56]
    mov ecx, [ebp - 68]
    mov [eax], cl
    ;; load t17, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 72], ecx
    ;; load t18, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 76], ecx
    ;; add t19, 3, t18
    mov eax, 3
    add eax, [ebp - 76]
    mov [ebp - 80], eax
    ;; load t20, value, 
    mov ecx, [ebp + 16]
    mov [ebp - 84], ecx
    ;; shr t21, t20, 24
    mov eax, [ebp - 84]
    mov cl, 24
    shr eax, cl
    mov [ebp - 88], eax
    ;; and t22, t21, 255
    mov eax, [ebp - 88]
    and eax, 255
    mov [ebp - 92], eax
    ;; setchar t17, t19, t22
    mov eax, [ebp - 72]
    add eax, [ebp - 80]
    mov ecx, [ebp - 92]
    mov [eax], cl
    ;; load t23, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 96], ecx
    ;; return return_value, t23, 
    mov eax, [ebp - 96]
    mov esp, ebp
    pop ebp
    ret
;; defun label, getint, 
getint:
    push ebp
    mov ebp, esp
    sub esp, 136
    ;; load t0, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; load t1, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 8], ecx
    ;; add t2, t1, 0
    mov eax, [ebp - 8]
    add eax, 0
    mov [ebp - 12], eax
    ;; add t3, t0, t2
    mov eax, [ebp - 4]
    add eax, [ebp - 12]
    mov [ebp - 16], eax
    ;; deref t4, t3, 
    mov eax, [ebp - 16]
    movzx ebx, byte [eax]
    mov [ebp - 20], ebx
    ;; assign b0, t4, 
    mov ecx, [ebp - 20]
    mov [ebp - 24], ecx
    ;; load t5, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 28], ecx
    ;; load t6, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 32], ecx
    ;; add t7, t6, 1
    mov eax, [ebp - 32]
    add eax, 1
    mov [ebp - 36], eax
    ;; add t8, t5, t7
    mov eax, [ebp - 28]
    add eax, [ebp - 36]
    mov [ebp - 40], eax
    ;; deref t9, t8, 
    mov eax, [ebp - 40]
    movzx ebx, byte [eax]
    mov [ebp - 44], ebx
    ;; assign b1, t9, 
    mov ecx, [ebp - 44]
    mov [ebp - 48], ecx
    ;; load t10, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 52], ecx
    ;; load t11, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 56], ecx
    ;; add t12, t11, 2
    mov eax, [ebp - 56]
    add eax, 2
    mov [ebp - 60], eax
    ;; add t13, t10, t12
    mov eax, [ebp - 52]
    add eax, [ebp - 60]
    mov [ebp - 64], eax
    ;; deref t14, t13, 
    mov eax, [ebp - 64]
    movzx ebx, byte [eax]
    mov [ebp - 68], ebx
    ;; assign b2, t14, 
    mov ecx, [ebp - 68]
    mov [ebp - 72], ecx
    ;; load t15, ptr, 
    mov ecx, [ebp + 8]
    mov [ebp - 76], ecx
    ;; load t16, index, 
    mov ecx, [ebp + 12]
    mov [ebp - 80], ecx
    ;; add t17, t16, 3
    mov eax, [ebp - 80]
    add eax, 3
    mov [ebp - 84], eax
    ;; add t18, t15, t17
    mov eax, [ebp - 76]
    add eax, [ebp - 84]
    mov [ebp - 88], eax
    ;; deref t19, t18, 
    mov eax, [ebp - 88]
    movzx ebx, byte [eax]
    mov [ebp - 92], ebx
    ;; assign b3, t19, 
    mov ecx, [ebp - 92]
    mov [ebp - 96], ecx
    ;; load t20, b0, 
    mov ecx, [ebp - 24]
    mov [ebp - 100], ecx
    ;; load t21, b1, 
    mov ecx, [ebp - 48]
    mov [ebp - 104], ecx
    ;; shl t22, t21, 8
    mov eax, [ebp - 104]
    mov cl, 8
    shl eax, cl
    mov [ebp - 108], eax
    ;; add t23, t20, t22
    mov eax, [ebp - 100]
    add eax, [ebp - 108]
    mov [ebp - 112], eax
    ;; load t24, b1, 
    mov ecx, [ebp - 48]
    mov [ebp - 116], ecx
    ;; shl t25, t24, 16
    mov eax, [ebp - 116]
    mov cl, 16
    shl eax, cl
    mov [ebp - 120], eax
    ;; add t26, t23, t25
    mov eax, [ebp - 112]
    add eax, [ebp - 120]
    mov [ebp - 124], eax
    ;; load t27, b1, 
    mov ecx, [ebp - 48]
    mov [ebp - 128], ecx
    ;; shl t28, t27, 24
    mov eax, [ebp - 128]
    mov cl, 24
    shl eax, cl
    mov [ebp - 132], eax
    ;; add t29, t26, t28
    mov eax, [ebp - 124]
    add eax, [ebp - 132]
    mov [ebp - 136], eax
    ;; return return_value, t29, 
    mov eax, [ebp - 136]
    mov esp, ebp
    pop ebp
    ret
;; defun label, alloc_string, 
alloc_string:
    push ebp
    mov ebp, esp
    sub esp, 72
    ;; load t0, len, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; add t1, t0, 9
    mov eax, [ebp - 4]
    add eax, 9
    mov [ebp - 8], eax
    ;; assign total_size, t1, 
    mov ecx, [ebp - 8]
    mov [ebp - 12], ecx
    ;; load t2, total_size, 
    mov ecx, [ebp - 12]
    mov [ebp - 16], ecx
    ;; malloc t3, t2, 
    mov eax, [heap_ptr]
    mov [ebp - 20], eax
    mov ecx, [ebp - 16]
    add eax, ecx
    mov [heap_ptr], eax
    ;; assign ptr, t3, 
    mov ecx, [ebp - 20]
    mov [ebp - 24], ecx
    ;; load t4, ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 28], ecx
    ;; load t5, len, 
    mov ecx, [ebp + 8]
    mov [ebp - 32], ecx
    ;; push , t5, 
    push dword [ebp - 32]
    ;; push , 0, 
    mov eax, 0
    push dword eax
    ;; push , t4, 
    push dword [ebp - 28]
    ;; call , setint, 
    call setint
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t6, , 
    mov [ebp - 36], eax
    ;; load t7, ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 40], ecx
    ;; load t8, len, 
    mov ecx, [ebp + 8]
    mov [ebp - 44], ecx
    ;; push , t8, 
    push dword [ebp - 44]
    ;; push , 4, 
    mov eax, 4
    push dword eax
    ;; push , t7, 
    push dword [ebp - 40]
    ;; call , setint, 
    call setint
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t9, , 
    mov [ebp - 48], eax
    ;; load t10, ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 52], ecx
    ;; add t11, t10, 8
    mov eax, [ebp - 52]
    add eax, 8
    mov [ebp - 56], eax
    ;; assign str_ptr, t11, 
    mov ecx, [ebp - 56]
    mov [ebp - 60], ecx
    ;; load t12, str_ptr, 
    mov ecx, [ebp - 60]
    mov [ebp - 64], ecx
    ;; load t13, len, 
    mov ecx, [ebp + 8]
    mov [ebp - 68], ecx
    ;; setchar t12, t13, 0
    mov eax, [ebp - 64]
    add eax, [ebp - 68]
    mov ecx, 0
    mov [eax], cl
    ;; load t14, str_ptr, 
    mov ecx, [ebp - 60]
    mov [ebp - 72], ecx
    ;; return return_value, t14, 
    mov eax, [ebp - 72]
    mov esp, ebp
    pop ebp
    ret
;; defun label, get_length, 
get_length:
    push ebp
    mov ebp, esp
    sub esp, 16
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; sub t1, t0, 8
    mov eax, [ebp - 4]
    sub eax, 8
    mov [ebp - 8], eax
    ;; load t2, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 12], ecx
    ;; push , 0, 
    mov eax, 0
    push dword eax
    ;; push , t2, 
    push dword [ebp - 12]
    ;; call , getint, 
    call getint
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t3, , 
    mov [ebp - 16], eax
    ;; return return_value, t3, 
    mov eax, [ebp - 16]
    mov esp, ebp
    pop ebp
    ret
;; defun label, get_capacity, 
get_capacity:
    push ebp
    mov ebp, esp
    sub esp, 16
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; sub t1, t0, 4
    mov eax, [ebp - 4]
    sub eax, 4
    mov [ebp - 8], eax
    ;; load t2, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 12], ecx
    ;; push , 0, 
    mov eax, 0
    push dword eax
    ;; push , t2, 
    push dword [ebp - 12]
    ;; call , getint, 
    call getint
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t3, , 
    mov [ebp - 16], eax
    ;; return return_value, t3, 
    mov eax, [ebp - 16]
    mov esp, ebp
    pop ebp
    ret
;; defun label, set_length, 
set_length:
    push ebp
    mov ebp, esp
    sub esp, 28
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; sub t1, t0, 8
    mov eax, [ebp - 4]
    sub eax, 8
    mov [ebp - 8], eax
    ;; load t2, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 12], ecx
    ;; load t3, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 16], ecx
    ;; push , t3, 
    push dword [ebp - 16]
    ;; push , 0, 
    mov eax, 0
    push dword eax
    ;; push , t2, 
    push dword [ebp - 12]
    ;; call , setint, 
    call setint
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t4, , 
    mov [ebp - 20], eax
    ;; load t5, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 24], ecx
    ;; add t6, t5, 8
    mov eax, [ebp - 24]
    add eax, 8
    mov [ebp - 28], eax
    ;; return return_value, t6, 
    mov eax, [ebp - 28]
    mov esp, ebp
    pop ebp
    ret
;; defun label, append_char__grow_string, 
append_char__grow_string:
    push ebp
    mov ebp, esp
    sub esp, 96
    ;; load t0, cap, 
    mov ecx, [ebp + 16]
    mov [ebp - 4], ecx
    ;; imul t1, t0, 2
    mov eax, [ebp - 4]
    imul eax, 2
    mov [ebp - 8], eax
    ;; assign new_cap, t1, 
    mov ecx, [ebp - 8]
    mov [ebp - 12], ecx
    ;; load t2, new_cap, 
    mov ecx, [ebp - 12]
    mov [ebp - 16], ecx
    ;; push , t2, 
    push dword [ebp - 16]
    ;; call , alloc_string, 
    call alloc_string
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t3, , 
    mov [ebp - 20], eax
    ;; assign new_ptr, t3, 
    mov ecx, [ebp - 20]
    mov [ebp - 24], ecx
    ;; load t4, new_ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 28], ecx
    ;; load t5, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 32], ecx
    ;; load t6, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 36], ecx
    ;; push , t6, 
    push dword [ebp - 36]
    ;; push , t5, 
    push dword [ebp - 32]
    ;; push , t4, 
    push dword [ebp - 28]
    ;; call , memcpy, 
    call memcpy
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t7, , 
    mov [ebp - 40], eax
    ;; load t8, new_ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 44], ecx
    ;; load t9, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 48], ecx
    ;; push , t9, 
    push dword [ebp - 48]
    ;; push , t8, 
    push dword [ebp - 44]
    ;; call , set_length, 
    call set_length
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t10, , 
    mov [ebp - 52], eax
    ;; load t11, new_ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 56], ecx
    ;; load t12, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 60], ecx
    ;; load t13, ch, 
    mov ecx, [ebp + 20]
    mov [ebp - 64], ecx
    ;; setchar t11, t12, t13
    mov eax, [ebp - 56]
    add eax, [ebp - 60]
    mov ecx, [ebp - 64]
    mov [eax], cl
    ;; load t14, new_ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 68], ecx
    ;; load t15, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 72], ecx
    ;; add t16, t15, 1
    mov eax, [ebp - 72]
    add eax, 1
    mov [ebp - 76], eax
    ;; setchar t14, t16, 0
    mov eax, [ebp - 68]
    add eax, [ebp - 76]
    mov ecx, 0
    mov [eax], cl
    ;; load t17, new_ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 80], ecx
    ;; load t18, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 84], ecx
    ;; add t19, t18, 1
    mov eax, [ebp - 84]
    add eax, 1
    mov [ebp - 88], eax
    ;; push , t19, 
    push dword [ebp - 88]
    ;; push , t17, 
    push dword [ebp - 80]
    ;; call , set_length, 
    call set_length
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t20, , 
    mov [ebp - 92], eax
    ;; load t21, new_ptr, 
    mov ecx, [ebp - 24]
    mov [ebp - 96], ecx
    ;; return return_value, t21, 
    mov eax, [ebp - 96]
    mov esp, ebp
    pop ebp
    ret
;; defun label, append_char__else, 
append_char__else:
    push ebp
    mov ebp, esp
    sub esp, 44
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; load t1, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 8], ecx
    ;; load t2, ch, 
    mov ecx, [ebp + 16]
    mov [ebp - 12], ecx
    ;; setchar t0, t1, t2
    mov eax, [ebp - 4]
    add eax, [ebp - 8]
    mov ecx, [ebp - 12]
    mov [eax], cl
    ;; load t3, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 16], ecx
    ;; load t4, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 20], ecx
    ;; add t5, t4, 1
    mov eax, [ebp - 20]
    add eax, 1
    mov [ebp - 24], eax
    ;; setchar t3, t5, 0
    mov eax, [ebp - 16]
    add eax, [ebp - 24]
    mov ecx, 0
    mov [eax], cl
    ;; load t6, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 28], ecx
    ;; load t7, len, 
    mov ecx, [ebp + 12]
    mov [ebp - 32], ecx
    ;; add t8, t7, 1
    mov eax, [ebp - 32]
    add eax, 1
    mov [ebp - 36], eax
    ;; push , t8, 
    push dword [ebp - 36]
    ;; push , t6, 
    push dword [ebp - 28]
    ;; call , set_length, 
    call set_length
    ;; add_esp , 8, 
    add esp, 8
    ;; assign t9, , 
    mov [ebp - 40], eax
    ;; load t10, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 44], ecx
    ;; return return_value, t10, 
    mov eax, [ebp - 44]
    mov esp, ebp
    pop ebp
    ret
;; defun label, append_char, 
append_char:
    push ebp
    mov ebp, esp
    sub esp, 76
    ;; load t0, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 4], ecx
    ;; push , t0, 
    push dword [ebp - 4]
    ;; call , get_length, 
    call get_length
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t1, , 
    mov [ebp - 8], eax
    ;; assign len, t1, 
    mov ecx, [ebp - 8]
    mov [ebp - 12], ecx
    ;; load t2, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 16], ecx
    ;; push , t2, 
    push dword [ebp - 16]
    ;; call , get_capacity, 
    call get_capacity
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t3, , 
    mov [ebp - 20], eax
    ;; assign cap, t3, 
    mov ecx, [ebp - 20]
    mov [ebp - 24], ecx
    ;; load t4, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 28], ecx
    ;; load t5, cap, 
    mov ecx, [ebp - 24]
    mov [ebp - 32], ecx
    ;; geq t6, t4, t5
    mov eax, [ebp - 28]
    cmp eax, [ebp - 32]
    setge al
    movzx eax, al
    mov [ebp - 36], eax
    ;; if label_if_true_5, t6, label_if_false_5
    mov eax, [ebp - 36]
    cmp eax, 0
    je  label_if_false_5
    jmp label_if_true_5
    ;; label label_if_false_5, , 
label_if_false_5:
    ;; load t8, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 44], ecx
    ;; load t9, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 48], ecx
    ;; load t10, ch, 
    mov ecx, [ebp + 12]
    mov [ebp - 52], ecx
    ;; push , t10, 
    push dword [ebp - 52]
    ;; push , t9, 
    push dword [ebp - 48]
    ;; push , t8, 
    push dword [ebp - 44]
    ;; call , append_char__else, 
    call append_char__else
    ;; add_esp , 12, 
    add esp, 12
    ;; assign t11, , 
    mov [ebp - 56], eax
    ;; assign t7, t11, 
    mov ecx, [ebp - 56]
    mov [ebp - 40], ecx
    ;; jump label_if_end_5, , 
    jmp label_if_end_5
    ;; label label_if_true_5, , 
label_if_true_5:
    ;; load t12, str, 
    mov ecx, [ebp + 8]
    mov [ebp - 60], ecx
    ;; load t13, len, 
    mov ecx, [ebp - 12]
    mov [ebp - 64], ecx
    ;; load t14, cap, 
    mov ecx, [ebp - 24]
    mov [ebp - 68], ecx
    ;; load t15, ch, 
    mov ecx, [ebp + 12]
    mov [ebp - 72], ecx
    ;; push , t15, 
    push dword [ebp - 72]
    ;; push , t14, 
    push dword [ebp - 68]
    ;; push , t13, 
    push dword [ebp - 64]
    ;; push , t12, 
    push dword [ebp - 60]
    ;; call , append_char__grow_string, 
    call append_char__grow_string
    ;; add_esp , 16, 
    add esp, 16
    ;; assign t16, , 
    mov [ebp - 76], eax
    ;; assign t7, t16, 
    mov ecx, [ebp - 76]
    mov [ebp - 40], ecx
    ;; label label_if_end_5, , 
label_if_end_5:
    ;; return return_value, t7, 
    mov eax, [ebp - 40]
    mov esp, ebp
    pop ebp
    ret
;; defun label, main, 
main:
    push ebp
    mov ebp, esp
    sub esp, 4
    ;; push , 255, 
    mov eax, 255
    push dword eax
    ;; call , alloc_string, 
    call alloc_string
    ;; add_esp , 4, 
    add esp, 4
    ;; assign t0, , 
    mov [ebp - 4], eax
    ;; return return_value, t0, 
    mov eax, [ebp - 4]
    mov esp, ebp
    pop ebp
    ret
section .data
str_0: db "..........", 0
str_1: db "", 10, "", 0
str_2: db "..........", 0
list_ptr: dd list_memory ; pointer to next free cell
heap_ptr: dd heap ; pointer to heap begin
section .bss
list_memory: resb 8192; reserved (uninitialized!!) space for cons cells
heap: resb 8192; heap space

