

# Git How To Use
## Getting started

### Install git on your machine.
  - Linux: sudo apt-get install git -y
  - Windows: https://git-scm.com/downloads/win

Open VSCode and open the terminal. 

> git clone 'https://project.url'

project.url = Project URL you want to clone.

You now have a local copy of the project.

## Updating your local copy 

To update the local copy with the latest changes from the remote copy.
> git pull

## Saving your changes

To save your changes made to the local copy.
1. Add all the changes you made to the local copy.
> git add --all .

2. Commit the changes.
> git commit 

3. Push the changes to the remote copy.
> git push

Now the remote copy of the project is updated with your changes.




 To merge your changes with the latest version of the code.
1. Sets Git to use VSCode for writing your commit messages
  > git config --global core.editor "code --wait" 





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

# Project syntax 

Tutor and Anna from Ed allow C99 to be used in this project.

Comments can be like this.

> // This is a comment
