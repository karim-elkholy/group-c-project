
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


# Jordan 
# Use below commands to upload any changes you made 
1. Set Git to use VSCode for writing your commit messages
git config --global core.editor "code --wait" 

2. Updates our git repo with your work.(for git commit: expect to write msg. )
git add -A .
git commit 

3. Synchronizes local repo on ur pc with the remote repo on ur pc 
git push 



