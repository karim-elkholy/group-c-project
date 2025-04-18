
Build & run command 
Linux 
Windows

gcc -Wall -Werror -ansi -ansi -lm aes/*.c main.c -o main.o && ./main.o 
Windows
gcc -Wall -Werror -ansi -ansi -lm aes/*.c main.c -o main.exe; ./main.exe 

Testing commands 
Linux:
gcc -g -Wall -Werror -ansi -ansi -lm aes/*.c tests/*.c -o test.o && ./test.o
Windows:
gcc -g -Wall -Werror -ansi -ansi -lm aes/*.c tests/*.c -o test.exe; ./test.exe

Debugging commands
Linux-only
gcc -g -Wall -Werror -ansi -ansi -lm aes/*.c main.c -o main.o && valgrind ./main.o
