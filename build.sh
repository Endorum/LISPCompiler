# <path to input file> <output name> paths are from where this script is
./build/LISPCompiler $1 "$2.asm"
nasm -f elf32 "$2.asm" -o "$2.o"
i686-elf-ld -o out "$2.asm.o"
rm -f *.o