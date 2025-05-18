cmake --build build       
./build/LISPCompiler test.lisp "out.asm" > out.txt
nasm -f elf32 out.asm -o out.o
i686-elf-ld -o out out.o 
multipass transfer out linux:/home/ubuntu/main