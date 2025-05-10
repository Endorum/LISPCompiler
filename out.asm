
%include "../stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
; label defun test
test:
    pop esi
    mov edi, return_address
    mov [edi], esi
    push esi
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 52
    ; t0 = 5
    mov eax, 5
    mov [ebp - 4], eax
    ; x store t0
    mov eax, [ebp - 4]
    mov [ebp + 8], eax
    ; t1 = 6
    mov eax, 6
    mov [ebp - 8], eax
    ; y store t1
    mov eax, [ebp - 8]
    mov [ebp + 12], eax
    ; t2 load x
    mov eax, [ebp + 8]
    mov [ebp - 12], eax
    ; t3 load y
    mov eax, [ebp + 12]
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
    mov eax, str_0
    mov [ebp - 36], eax
    ; retValuet7 = t8
    mov eax, [ebp - 36]
    mov [ebp + 16], eax
    ; endLabelt7 jump 
    jmp endLabelt7
    ; falseLabelt7 label 
falseLabelt7:
    ; t9 loadstr no
    mov eax, str_1
    mov [ebp - 40], eax
    ; retValuet7 = t9
    mov eax, [ebp - 40]
    mov [ebp + 16], eax
    ; endLabelt7 label 
endLabelt7:
    ; return_value return retValuet7
    mov eax, [ebp + 16]
    ; Stack frame cleanup13
    add esp, 52
    mov edi, return_address
    mov esi, [edi]
    pop ebp
    push esi
    ret
; label defun main
main:
    pop esi
    mov edi, return_address
    mov [edi], esi
    push esi
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
    mov edi, return_address
    mov esi, [edi]
    pop ebp
    push esi
    ret

section .data
return_address: dd main
list_memory: times 1024 db 0 ; reserved space for cons cells
list_ptr: dd list_memory ; pointer to next free cell
str_0: db "yes", 0
str_1: db "no", 0
