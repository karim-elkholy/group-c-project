
# Getting started

# Install git on your machine.
  - Linux: sudo apt-get install git -y
  - Windows: https://git-scm.com/downloads/win

Open VSCode and open the terminal. 

> git clone 'https://project.url'

project.url = Project URL you want to clone.

You now have a local copy of the project.

## Setting up

### Set your email & name for local commits
> git config --global user.email "your.email@student.edu.au"

> git config --global user.name "Your Name"

### Set your account for github commits
> gh auth login

Instructions on how to install 'gh' can be found below.
https://docs.github.com/en/github-cli/github-cli/quickstart

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

### Bonus - Updating commit message editor

The below command sets Git to use VSCode for writing your commit messages.
> git config --global core.editor "code --wait" 

