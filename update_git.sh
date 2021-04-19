#!/bin/bash

### .gitignore ###

# Create brand new .gitignore
rm .gitignore
touch .gitignore
chmod u=rwx .gitignore


# Fill .gitignore
echo "
# LaTeX
*.aux
*.fdb_latexmk
*.fls
*.log
*.out
*.synctex.gz

# Python
__pycache__
tempCodeRunnerFile.py
*.pyc

# Some other people's projects
/vqe
/tif285
/'manim-master (3Blue1Brown)'
/'Navigating a social world with robot partners: a quantitative cartography of the Uncanny Valley'

# Some junk
/HPC/assignment 1/writing to and reading from hdd and ssd/include

# Nestled repos
*/.git
" >> .gitignore

# Add every file larger than X MB to .gitignore
cd ~/Workspace
find -L . -size +20M | sed 's|^\./||g' | cat >> .gitignore

# Copy current neovim configs into workspace
cp /home/simon/.config/nvim/init.vim /home/simon/Workspace/.init.vim


### Push to git ###
git add --ignore-errors --all
git commit -m "$(date)"
# git remote set-url origin https://SimonKvantdator@github.com/SimonKvantdator/Workspace.git
# git remote add usb /media/simon/E908-2BF9/Workspace/
git branch -M main
git push -u origin main
if [ -d "/media/simon/E908-2BF9/Documents/" ]; then
	git push usb
	echo "pushed to remote usb"
fi
