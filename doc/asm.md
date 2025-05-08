this is a temp

i think i only need to check for the different ops and generate accordingly.
ops:

= <- assignment

add
sub
mul
div
and
or
xor
neg
le
gr
eq
not

store
load

print

cons
car
cdr
null?

string_to_list
list_to_string

push
pop

if
jump
call

read_file
scan_user_input
listLength

i already want to think about how im generating the assembler code

e.g. (+ 5 6) would be somthing like
t1 = 5
t2 = 6
t3 = t1 + t2


ill work with stack offset variables.
e.g. for the IR code above it would result in something like

mov eax, 5
mov [ebp - 4], eax

mov eax, 6
mov [ebp - 8], eax

mov eax, [ebp - 4]
add eax, [ebp - 8]
mov [ebp - 12], eax


