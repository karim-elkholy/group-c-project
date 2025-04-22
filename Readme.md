
# Project syntax 

Tutor and Anna from Ed allow C99 to be used in this project.

Comments can be like this.

> // This is a comment

# Git How To Use

Will be explaining Git using a video game analogy.

Git allows you to make saves of your code similar to a video game.

 Use below commands to upload any changes you made 
1. Sets Git to use VSCode for writing your commit messages
  > git config --global core.editor "code --wait" 

2. Updates our git repo with your work.

Marks all files in current dir aka(.) to be included in the savefile.
>git add --all .

Perform the actual save aka git will ask you what to name your savefile or a description of the changes you made.
>git commit 

3. Synchronizes local repo on ur pc with the remote repo on ur pc 

This tells Git to synchronize your local save file with the cloud save file.

Same as pushing your local game save file to Steam cloud save files.

Now everyone in group can use your save files.
>git push 



# Ignore this for now
# Below is work in progress.
## Will update it later after our tutorial to match our codebase.

Build & run commands listed below  from lecture notes .
We should adhere to these to avoid losing marks.
For now I am running the code using CMakeLists.txt, will create Makefile later.

Linux 
>gcc -Wall -Werror -ansi -ansi -lm aes/*.c main.c -o main.o && ./main.o 
Windows
>gcc -Wall -Werror -ansi -ansi -lm aes/*.c main.c -o main.exe; ./main.exe 


# Debugging commands(Linux-only)

Valgrind installation differs by distro.

Figure out how to install valgrind yourself.
  
Step 0. If you use debian/ubuntu/mint do 
  > sudo apt-get install valgrind -y 

Step 1. Compile your program into an executable with debug flags turned on 
> gcc -Wall -Werror -c99 -lm aes/*.c main.c -o main.o && ./main.o 

Step 2. Run valgrind --show-leak-kinds=all --leak-check=full ./test.o 
> valgrind --show-leak-kinds=all --leak-check=full ./main.o 

