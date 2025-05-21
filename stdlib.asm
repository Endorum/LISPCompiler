global print

section .text
_internal_print_string:
    ; Input: eax = pointer to null-terminated string

    push ebp
    mov ebp, esp

    push eax             ; save pointer to string
    call _internal_strlen ; get length in eax
    pop ebx              ; restore string pointer to ebx

    mov ecx, ebx         ; ecx = pointer to string
    mov edx, eax         ; edx = length
    mov ebx, 1           ; stdout (file descriptor)
    mov eax, 4           ; syscall: sys_write
    int 0x80

    pop ebp
    ret


_internal_strlen:
    ; Input: eax = pointer to string
    ; Output: eax = length

    push ebp
    mov ebp, esp

    push edi
    mov edi, eax
    xor eax, eax

.len_loop:
    cmp byte [edi], 0
    je .done
    inc edi
    inc eax
    jmp .len_loop

.done:
    pop edi
    pop ebp
    ret




malloc:
    ; input: size in eax
    ; output: pointer in eax

    push ebx
    push ecx
    push edx

    add eax, 8              ; account for header

    mov ebx, [free_list]
    mov ecx, 0              ; previous pointer

.search_loop:
    cmp ebx, 0
    je .no_block            ; out of memory

    mov edx, [ebx]          ; edx = size
    cmp edx, eax
    jb .next_block          ; not enough space

    ; found a good block!
    ; if (edx - eax) > minimal_block_size → split
    ; else → use full block

    ; calculate remaining space after allocation
    mov esi, edx        ; esi = original block size
    sub esi, eax        ; esi = remaining after allocation
    cmp esi, 16         ; if enough room for a new block (8 header + at least 8 data)
    jl .no_split        ; if not, don't split

    ; split: create new block after allocated one
    mov edi, ebx        ; edi = current block ptr
    add edi, eax        ; edi = new block address

    mov [edi], esi      ; store remaining size
    mov edx, [ebx+4]    ; get next pointer of current block
    mov [edi+4], edx    ; copy it to new block

    ; update free list to point to new block
    cmp ecx, 0
    je .update_head_split
    mov [ecx+4], edi
    jmp .return_ptr

.update_head_split:
    mov [free_list], edi
    jmp .return_ptr

.no_split:

    ; unlink from freelist
    mov edx, [ebx+4]        ; edx = next
    cmp ecx, 0
    je .update_head
    mov [ecx+4], edx
    jmp .return_ptr
.update_head:
    mov [free_list], edx

.return_ptr:
    add ebx, 8              ; skip header
    mov eax, ebx
    pop edx
    pop ecx
    pop ebx
    ret

.next_block:
    mov ecx, ebx
    mov ebx, [ebx+4]
    jmp .search_loop

.no_block:
    ; could just grow heap here or return 0
    mov eax, 0
    pop edx
    pop ecx
    pop ebx
    ret


free:
    ; input: ptr in eax
    sub eax, 8

    mov ecx, [free_list]
    mov [eax+4], ecx
    mov [free_list], eax
    ret
