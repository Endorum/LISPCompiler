
%include "../stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret
; label defun main
main:
    ; Stack frame
    push ebp
    mov ebp, esp
    sub esp, 24
    ; t0 loadstr 1\n
    mov eax, str_0
    mov [ebp - 4], eax
    ; t1 loadstr 2
    mov eax, str_1
    mov [ebp - 8], eax
    ; t2 loadstr 3
    mov eax, str_2
    mov [ebp - 12], eax
    ; t3 cons t1 t2
    ; load car (src1) into eax
    mov eax, [ebp - 8]
    ; store car into [list_ptr]
    mov ebx, [list_ptr]
    mov [ebx], eax
    ; load cdr (src2) into eax
    mov eax, [ebp - 12]
    ; Load cdr into [list_ptr + 4]
    mov ebx, [list_ptr]
    mov [ebx + 4], eax
    ; Store address of cons cell into dst
    mov eax, [list_ptr]
    mov [ebp - 16], eax
    ; Increment list pointer by 8 for next allocation
    add dword [list_ptr], 8
    ; t4 cons t0 t3
    ; load car (src1) into eax
    mov eax, [ebp - 4]
    ; store car into [list_ptr]
    mov ebx, [list_ptr]
    mov [ebx], eax
    ; load cdr (src2) into eax
    mov eax, [ebp - 16]
    ; Load cdr into [list_ptr + 4]
    mov ebx, [list_ptr]
    mov [ebx + 4], eax
    ; Store address of cons cell into dst
    mov eax, [list_ptr]
    mov [ebp - 20], eax
    ; Increment list pointer by 8 for next allocation
    add dword [list_ptr], 8
    ; t5 car t4
    mov ebx, [ebp - 20]
    mov eax, [ebx]
    mov [ebp - 24], eax
    ; stdout print t5
    mov eax, [ebp - 24]
    call _internal_print_string    ; return_value return t5
    mov ebx, [ebp - 24]
    mov eax, ebx
    mov [ebp + 8], eax
    ; Stack frame cleanup
    mov esp, ebp
    pop ebp
    ret

section .data
list_memory: times 65336 db 0 ; reserved space for cons cells
list_ptr: dd list_memory ; pointer to next free cell
str_0: db "1", 10, "", 0
str_1: db "2", 0
str_2: db "3", 0
