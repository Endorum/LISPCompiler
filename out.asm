
%include "../stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
; label defun add
add:
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 12
    ; y getArg stack
    mov eax, [ebp - 8]    ; x getArg stack
    mov eax, [ebp - 12]    ; t0 load x
    mov eax, [ebp - 12]
    mov [ebp + 4], eax
    ; t1 load y
    mov eax, [ebp - 8]
    mov [ebp + 8], eax
    ; t2 add t0 t1
    mov eax, [ebp + 4]
    add eax, [ebp + 8]
    mov [ebp + 12], eax
    ; return_value return t2
    mov ebx, [ebp + 12]
    mov eax, ebx
    mov [ebp - 20], eax
    ; Stack frame cleanup
    mov esp, ebp
    pop ebp
    ret
; label defun main
main:
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 12
    ; t0 = 5
    mov eax, 5
    mov [ebp + 4], eax
    ; stack push t0
    mov eax, [ebp + 4]
    push eax
    ; t1 = 6
    mov eax, 6
    mov [ebp + 8], eax
    ; stack push t1
    mov eax, [ebp + 8]
    push eax
    ; function call call add
    call add
    ; t2 = return_value
    mov [ebp + 12], eax
    ; return_value return t2
    mov ebx, [ebp + 12]
    mov eax, ebx
    mov [ebp - 20], eax
    ; Stack frame cleanup
    mov esp, ebp
    pop ebp
    ret

section .data
list_memory: times 1024 db 0 ; reserved space for cons cells
list_ptr: dd list_memory ; pointer to next free cell
