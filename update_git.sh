#!/bin/bash

### .gitignore ###

# Create brand new .gitignore
rm .gitignore
touch .gitignore

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

*/.git
" >> .gitignore

# Add every file larger than X MB to .gitignore
find . -size +1M | sed 's|^\./||g' | cat >> .gitignore

# Copy current neovim configs into workspace
cp /home/simon/.config/nvim/init.vim /home/simon/Workspace/.init.vim


### Push to git ###
git add --all
git commit -m "$(date)"
git remote add origin https://github.com/SimonKvantdator/Workspace.git
git branch -M main
git push -u origin main

