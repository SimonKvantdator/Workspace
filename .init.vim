if has('win32') || has('win64')
	let g:plugged_home = '~/AppData/Local/nvim/plugged'
else
	let g:plugged_home = '~/.vim/plugged'
endif

" Plugins List
call plug#begin(g:plugged_home)
	" UI related
	Plug 'chriskempson/base16-vim'
	Plug 'vim-airline/vim-airline'
	Plug 'vim-airline/vim-airline-themes'
    Plug 'franbach/miramare'

	" Better Visual Guide
	Plug 'Yggdroot/indentLine'

	" syntax check
	Plug 'w0rp/ale'
	
	" Autocomplete
	Plug 'ncm2/ncm2'
	Plug 'roxma/nvim-yarp'
	Plug 'ncm2/ncm2-bufword'
	Plug 'ncm2/ncm2-path'
	Plug 'ncm2/ncm2-jedi'

	" Comment out blocks
	Plug 'preservim/nerdcommenter'

	" Formater
	Plug 'Chiel92/vim-autoformat'
	
	" LaTeX
	" Plug 'vim-latex/vim-latex' TODO: use latex-suite
	Plug 'lervag/vimtex'
	Plug 'Konfekt/FastFold'
	Plug 'matze/vim-tex-fold'

	" LargeFile
	Plug 'vim-scripts/LargeFile'
call plug#end()
filetype plugin indent on
filetype indent off
filetype plugin on


" Configurations Part
" UI configuration
syntax on
syntax enable

" Use <ctrl>+j/k/h/l to switch between panes in split mode
map <C-j> <C-W>j
map <C-k> <C-W>k
map <C-h> <C-W>h
map <C-l> <C-W>l

" colorscheme
" True Color Support if it's avaiable in terminal
if has("termguicolors")
	set termguicolors
endif
let g:miramare_enable_italic = 1
let g:miramare_disable_italic_comment = 1
colorscheme miramare
" opaque background
" highlight Normal guibg=none
" highlight NonText guibg=none


if has("gui_running")
	set guicursor=n-v-c-sm:block,i-ci-ve:block,r-cr-o:blocks
endif
set number
set relativenumber
set hidden
set mouse=a
set noshowmode
set noshowmatch
set nolazyredraw
" Turn off backup
set nobackup
set noswapfile
set nowritebackup
" Search configuration
set ignorecase                    " ignore case when searching
set smartcase                     " turn on smartcase
" Tab and Indent configuration
set noexpandtab
set nocopyindent
set nopreserveindent
set softtabstop=0
set shiftwidth=4
set tabstop=4

" vim-autoformat
noremap <F3> :Autoformat<CR>

" NCM2
augroup NCM2
	autocmd!
	" enable ncm2 for all buffers
	autocmd BufEnter * call ncm2#enable_for_buffer()
	" :help Ncm2PopupOpen for more information
	set completeopt=noinsert,menuone,noselect
	" When the <Enter> key is pressed while the popup menu is visible, it only
	" hides the menu. Use this mapping to close the menu and also start a new line.
	inoremap <expr> <CR> (pumvisible() ? "\<c-y>\<cr>" : "\<CR>")
	
	" uncomment this block if you use vimtex for LaTex
	autocmd Filetype tex call ncm2#register_source({
		\ 'name': 'vimtex',
		\ 'priority': 8,
		\ 'scope': ['tex'],
		\ 'mark': 'tex',
		\ 'word_pattern': '\w+',
		\ 'complete_pattern': g:vimtex#re#ncm2,
		\ 'on_complete': ['ncm2#on_complete#omni', 'vimtex#complete#omnifunc'],
	\ })
	
	" cycle through completion list with tab/shift+tab
	inoremap <expr> <tab> pumvisible() ? "\<c-n>" : "\<tab>"
	inoremap <expr> <s-tab> pumvisible() ? "\<c-p>" : "\<tab>"
augroup END

" Ale
let g:ale_lint_on_enter = 0
let g:ale_lint_on_text_changed = 'never'
let g:ale_echo_msg_error_str = 'E'
let g:ale_echo_msg_warning_str = 'W'
let g:ale_echo_msg_format = '[%linter%] %s [%severity%]'
let g:ale_linters = {'python': ['flake8']}
let g:ale_python_flake8_options = '--ignore=E501' " Disable error message about lines being too long

" Airline
let g:airline_left_sep  = ''
let g:airline_right_sep = ''
let g:airline#extensions#ale#enabled = 1
let airline#extensions#ale#error_symbol = 'E:'
let airline#extensions#ale#warning_symbol = 'W:'
let g:airline_theme = 'miramare'

let mapleader=","
set timeout timeoutlen=1500

" Run python scripts with f5
autocmd FileType python map <buffer> <F5> :w<CR>:exec '!python3' shellescape(@%, 1)<CR>
autocmd FileType python imap <buffer> <F5> <esc>:w<CR>:exec '!python3' shellescape(@%, 1)<CR>

" Compile and run C programs with f5
autocmd FileType c map <buffer> F5 :w<CR>:exec '!make && ./%:r'<CR>
autocmd FileType c imap <buffer> F5 <esc>:w<CR>:exec '!make && ./%:r'<CR>

" Vimtex Plugin settings
let g:tex_flavor  = 'latex'
let g:tex_conceal = ''
let g:vimtex_fold_manual = 1
let g:vimtex_latexmk_continuous = 1
let g:vimtex_compiler_progname = 'nvr'
let g:vimtex_view_method = 'general'
let g:vimtex_view_general_viewer = 'zathura'
let g:syntastic_tex_checkers = ['lacheck']

" Forward Searching in Zathura
function! SyncTexForward()
let linenumber=line(".")
let colnumber=col(".")
let filename=bufname("%")
let filenamePDF=filename[:-4]."pdf"
let execstr="!zathura --synctex-forward " . linenumber . ":" . colnumber . ":" . filename . " " . filenamePDF . "&>/dev/null &"
exec execstr 
endfunction
nmap  :call SyncTexForward()

" LargeFile settings
let g:LargeFile = 100 " How large (in MB) files should I use the LargeFile script for?
