
%include "../stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
; label defun digit_to_string
digit_to_string:
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 172
    ; t1 load x
    mov eax, [ebp + 20]
    mov [ebp - 8], eax
    ; t2 = 0
    mov eax, 0
    mov [ebp - 12], eax
    ; t3 eq t1 t2
    mov eax, [ebp - 8]
    cmp eax, [ebp - 12]
    sete al
    movzx eax, al
    mov [ebp - 16], eax
    ; cond_t0_clause_1 if t3 jump
    mov eax, [ebp - 16]
    cmp eax, 0
    jne cond_t0_clause_1
    ; cond_t0_next_1 jump 
    jmp cond_t0_next_1
    ; cond_t0_clause_1 label 
cond_t0_clause_1:
    ; t4 loadstr 0
    mov eax, str_0
    mov [ebp - 20], eax
    ; retValuet0 = t4
    mov eax, [ebp - 20]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_1 label 
cond_t0_next_1:
    ; t5 load x
    mov eax, [ebp + 20]
    mov [ebp - 24], eax
    ; t6 = 1
    mov eax, 1
    mov [ebp - 28], eax
    ; t7 eq t5 t6
    mov eax, [ebp - 24]
    cmp eax, [ebp - 28]
    sete al
    movzx eax, al
    mov [ebp - 32], eax
    ; cond_t0_clause_2 if t7 jump
    mov eax, [ebp - 32]
    cmp eax, 0
    jne cond_t0_clause_2
    ; cond_t0_next_2 jump 
    jmp cond_t0_next_2
    ; cond_t0_clause_2 label 
cond_t0_clause_2:
    ; t8 loadstr 1
    mov eax, str_1
    mov [ebp - 36], eax
    ; retValuet0 = t8
    mov eax, [ebp - 36]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_2 label 
cond_t0_next_2:
    ; t9 load x
    mov eax, [ebp + 20]
    mov [ebp - 40], eax
    ; t10 = 2
    mov eax, 2
    mov [ebp - 44], eax
    ; t11 eq t9 t10
    mov eax, [ebp - 40]
    cmp eax, [ebp - 44]
    sete al
    movzx eax, al
    mov [ebp - 48], eax
    ; cond_t0_clause_3 if t11 jump
    mov eax, [ebp - 48]
    cmp eax, 0
    jne cond_t0_clause_3
    ; cond_t0_next_3 jump 
    jmp cond_t0_next_3
    ; cond_t0_clause_3 label 
cond_t0_clause_3:
    ; t12 loadstr 2
    mov eax, str_2
    mov [ebp - 52], eax
    ; retValuet0 = t12
    mov eax, [ebp - 52]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_3 label 
cond_t0_next_3:
    ; t13 load x
    mov eax, [ebp + 20]
    mov [ebp - 56], eax
    ; t14 = 3
    mov eax, 3
    mov [ebp - 60], eax
    ; t15 eq t13 t14
    mov eax, [ebp - 56]
    cmp eax, [ebp - 60]
    sete al
    movzx eax, al
    mov [ebp - 64], eax
    ; cond_t0_clause_4 if t15 jump
    mov eax, [ebp - 64]
    cmp eax, 0
    jne cond_t0_clause_4
    ; cond_t0_next_4 jump 
    jmp cond_t0_next_4
    ; cond_t0_clause_4 label 
cond_t0_clause_4:
    ; t16 loadstr 3
    mov eax, str_3
    mov [ebp - 68], eax
    ; retValuet0 = t16
    mov eax, [ebp - 68]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_4 label 
cond_t0_next_4:
    ; t17 load x
    mov eax, [ebp + 20]
    mov [ebp - 72], eax
    ; t18 = 4
    mov eax, 4
    mov [ebp - 76], eax
    ; t19 eq t17 t18
    mov eax, [ebp - 72]
    cmp eax, [ebp - 76]
    sete al
    movzx eax, al
    mov [ebp - 80], eax
    ; cond_t0_clause_5 if t19 jump
    mov eax, [ebp - 80]
    cmp eax, 0
    jne cond_t0_clause_5
    ; cond_t0_next_5 jump 
    jmp cond_t0_next_5
    ; cond_t0_clause_5 label 
cond_t0_clause_5:
    ; t20 loadstr 4
    mov eax, str_4
    mov [ebp - 84], eax
    ; retValuet0 = t20
    mov eax, [ebp - 84]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_5 label 
cond_t0_next_5:
    ; t21 load x
    mov eax, [ebp + 20]
    mov [ebp - 88], eax
    ; t22 = 5
    mov eax, 5
    mov [ebp - 92], eax
    ; t23 eq t21 t22
    mov eax, [ebp - 88]
    cmp eax, [ebp - 92]
    sete al
    movzx eax, al
    mov [ebp - 96], eax
    ; cond_t0_clause_6 if t23 jump
    mov eax, [ebp - 96]
    cmp eax, 0
    jne cond_t0_clause_6
    ; cond_t0_next_6 jump 
    jmp cond_t0_next_6
    ; cond_t0_clause_6 label 
cond_t0_clause_6:
    ; t24 loadstr 5
    mov eax, str_5
    mov [ebp - 100], eax
    ; retValuet0 = t24
    mov eax, [ebp - 100]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_6 label 
cond_t0_next_6:
    ; t25 load x
    mov eax, [ebp + 20]
    mov [ebp - 104], eax
    ; t26 = 6
    mov eax, 6
    mov [ebp - 108], eax
    ; t27 eq t25 t26
    mov eax, [ebp - 104]
    cmp eax, [ebp - 108]
    sete al
    movzx eax, al
    mov [ebp - 112], eax
    ; cond_t0_clause_7 if t27 jump
    mov eax, [ebp - 112]
    cmp eax, 0
    jne cond_t0_clause_7
    ; cond_t0_next_7 jump 
    jmp cond_t0_next_7
    ; cond_t0_clause_7 label 
cond_t0_clause_7:
    ; t28 loadstr 6
    mov eax, str_6
    mov [ebp - 116], eax
    ; retValuet0 = t28
    mov eax, [ebp - 116]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_7 label 
cond_t0_next_7:
    ; t29 load x
    mov eax, [ebp + 20]
    mov [ebp - 120], eax
    ; t30 = 7
    mov eax, 7
    mov [ebp - 124], eax
    ; t31 eq t29 t30
    mov eax, [ebp - 120]
    cmp eax, [ebp - 124]
    sete al
    movzx eax, al
    mov [ebp - 128], eax
    ; cond_t0_clause_8 if t31 jump
    mov eax, [ebp - 128]
    cmp eax, 0
    jne cond_t0_clause_8
    ; cond_t0_next_8 jump 
    jmp cond_t0_next_8
    ; cond_t0_clause_8 label 
cond_t0_clause_8:
    ; t32 loadstr 7
    mov eax, str_7
    mov [ebp - 132], eax
    ; retValuet0 = t32
    mov eax, [ebp - 132]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_8 label 
cond_t0_next_8:
    ; t33 load x
    mov eax, [ebp + 20]
    mov [ebp - 136], eax
    ; t34 = 8
    mov eax, 8
    mov [ebp - 140], eax
    ; t35 eq t33 t34
    mov eax, [ebp - 136]
    cmp eax, [ebp - 140]
    sete al
    movzx eax, al
    mov [ebp - 144], eax
    ; cond_t0_clause_9 if t35 jump
    mov eax, [ebp - 144]
    cmp eax, 0
    jne cond_t0_clause_9
    ; cond_t0_next_9 jump 
    jmp cond_t0_next_9
    ; cond_t0_clause_9 label 
cond_t0_clause_9:
    ; t36 loadstr 8
    mov eax, str_8
    mov [ebp - 148], eax
    ; retValuet0 = t36
    mov eax, [ebp - 148]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_9 label 
cond_t0_next_9:
    ; t37 load x
    mov eax, [ebp + 20]
    mov [ebp - 152], eax
    ; t38 = 9
    mov eax, 9
    mov [ebp - 156], eax
    ; t39 eq t37 t38
    mov eax, [ebp - 152]
    cmp eax, [ebp - 156]
    sete al
    movzx eax, al
    mov [ebp - 160], eax
    ; cond_t0_clause_10 if t39 jump
    mov eax, [ebp - 160]
    cmp eax, 0
    jne cond_t0_clause_10
    ; cond_t0_next_10 jump 
    jmp cond_t0_next_10
    ; cond_t0_clause_10 label 
cond_t0_clause_10:
    ; t40 loadstr 9
    mov eax, str_9
    mov [ebp - 164], eax
    ; retValuet0 = t40
    mov eax, [ebp - 164]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_next_10 label 
cond_t0_next_10:
    ; cond_t0_else label 
cond_t0_else:
    ; t41 loadstr ERROR
    mov eax, str_10
    mov [ebp - 168], eax
    ; retValuet0 = t41
    mov eax, [ebp - 168]
    mov [ebp + 12], eax
    ; cond_t0_end jump 
    jmp cond_t0_end
    ; cond_t0_end label 
cond_t0_end:
    ; return_value return retValuet0
    mov eax, [ebp + 12]
    ; Stack frame cleanup43
    add esp, 172
    pop ebp
    ret
; label defun test
test:
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 52
    ; t0 = 5
    mov eax, 5
    mov [ebp - 4], eax
    ; x store t0
    mov eax, [ebp - 4]
    mov [ebp + 20], eax
    ; t1 = 6
    mov eax, 6
    mov [ebp - 8], eax
    ; y store t1
    mov eax, [ebp - 8]
    mov [ebp + 24], eax
    ; t2 load x
    mov eax, [ebp + 20]
    mov [ebp - 12], eax
    ; t3 load y
    mov eax, [ebp + 24]
    mov [ebp - 16], eax
    ; t4 add t2 t3
    mov eax, [ebp - 12]
    add eax, [ebp - 16]
    mov [ebp - 20], eax
    ; t5 = 11
    mov eax, 11
    mov [ebp - 24], eax
    ; t6 eq t4 t5
    mov eax, [ebp - 20]
    cmp eax, [ebp - 24]
    sete al
    movzx eax, al
    mov [ebp - 28], eax
    ; trueLabelt7 if t6 jump
    mov eax, [ebp - 28]
    cmp eax, 0
    jne trueLabelt7
    ; falseLabelt7 jump 
    jmp falseLabelt7
    ; trueLabelt7 label 
trueLabelt7:
    ; t8 loadstr yes
    mov eax, str_11
    mov [ebp - 36], eax
    ; retValuet7 = t8
    mov eax, [ebp - 36]
    mov [ebp + 28], eax
    ; endLabelt7 jump 
    jmp endLabelt7
    ; falseLabelt7 label 
falseLabelt7:
    ; t9 loadstr no
    mov eax, str_12
    mov [ebp - 40], eax
    ; retValuet7 = t9
    mov eax, [ebp - 40]
    mov [ebp + 28], eax
    ; endLabelt7 label 
endLabelt7:
    ; return_value return retValuet7
    mov eax, [ebp + 28]
    ; Stack frame cleanup13
    add esp, 52
    pop ebp
    ret
; label defun main
main:
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 4
    ; function call call test
    call test
    ; t0 = return_value
    mov [ebp - 4], eax
    ; stdout print t0
    mov eax, [ebp - 4]
    call _internal_print_string    ; return_value return t0
    mov eax, [ebp - 4]
    ; Stack frame cleanup1
    add esp, 4
    pop ebp
    ret

section .data
return_address: dd main
list_memory: times 1024 db 0 ; reserved space for cons cells
list_ptr: dd list_memory ; pointer to next free cell
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
str_11: db "yes", 0
str_12: db "no", 0
