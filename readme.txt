
Build & run commands listed below  from lecture notes .
We should adhere to these to avoid losing marks.
For now I am running the code using CMakeLists.txt, will create Makefile later.

Linux 
gcc -Wall -Werror -ansi -ansi -lm aes/*.c main.c -o main.o && ./main.o 
Windows
gcc -Wall -Werror -ansi -ansi -lm aes/*.c main.c -o main.exe; ./main.exe 
#-------------------------------------------------------------------------------

Debugging commands

Linux-only
Step 0. Figure out how to install valgrind yourself.
  Step 0a. If you use debian/ubuntu/mint do sudo apt-get install valgrind -y 
Step 1. Compile your program into an executable with debug flags turned on 
Step 2. Run valgrind --show-leak-kinds=all --leak-check=full ./test.o 


