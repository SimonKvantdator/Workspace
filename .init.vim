if has('win32') || has('win64')
	let g:plugged_home = '~/AppData/Local/nvim/plugged'
else
	let g:plugged_home = '~/.vim/plugged'
endif

set nocompatible " We don't care about being backwards compatibility
set wildmenu

" Set path for :find
set path+=./*
set path+=~/Documents/*
set path+=~/Workspace/*
set path+=~/.config/nvim " To be able to find init.vim
" set wildignore+=~/Workspace/gammal\ kod\ som\ jag\ kanske\ vill\ kolla\ på\ igen/*

" Netrw for visual file browsing
let g:netrw_banner=0 " Disable annoying banner
let g:netrw_altv=1 " Open splits to the right
let g:netrw_liststyle=3 " Tree view

" Plugins List
call plug#begin(g:plugged_home)
	" UI related
	Plug 'chriskempson/base16-vim'
	Plug 'vim-airline/vim-airline'
	Plug 'vim-airline/vim-airline-themes'
    Plug 'franbach/miramare'

	" Syntax check
	Plug 'w0rp/ale'
	
	" Autocomplete
	Plug 'ncm2/ncm2'
	Plug 'roxma/nvim-yarp'
	Plug 'ncm2/ncm2-bufword'
	Plug 'ncm2/ncm2-path'
	Plug 'ncm2/ncm2-jedi'

	" Comment out blocks
	Plug  'tpope/vim-commentary'

	" Formater
	Plug 'Chiel92/vim-autoformat'
	
	" LaTeX
	Plug 'lervag/vimtex'
	Plug 'Konfekt/FastFold'
	Plug 'matze/vim-tex-fold'

	" Snippets
	Plug 'sirver/ultisnips'
	Plug 'honza/vim-snippets'

	" Turn off syntax highlighting and such for large files
	Plug 'vim-scripts/LargeFile'

	" User defined modes
	Plug 'Iron-E/nvim-libmodal'
call plug#end()
filetype plugin indent on
filetype indent off
filetype plugin on

" There is a bug in UltiSnips that causes NeoVim to freeze a function key in insert mode. This is a workaround.
if has('nvim')
    au VimEnter * if exists('#UltiSnips_AutoTrigger')
        \ |     exe 'au! UltiSnips_AutoTrigger'
        \ |     aug! UltiSnips_AutoTrigger
        \ | endif
endif


" Configurations Part
" UI configuration
syntax on
syntax enable

" Do not highlight matching parentheses
let g:loaded_matchparen=1

" Use <ctrl>+j/k/h/l to switch between panes in split mode
map <C-j> <C-W>j
map <C-k> <C-W>k
map <C-h> <C-W>h
map <C-l> <C-W>l

" Colorscheme
if has("termguicolors") " True Color Support if it's avaiable in terminal
	set termguicolors
endif
colorscheme miramare
let g:miramare_enable_italic = 1
let g:miramare_disable_italic_comment = 1
" Opaque background
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
set ignorecase " Ignore case when searching
set smartcase " Turn on smartcase

" Tab and Indent configuration
set noexpandtab
set nocopyindent
set nopreserveindent
set softtabstop=0
set shiftwidth=4
set tabstop=4

set foldlevelstart=0 " Folds up to level 5 are open by default

" Vim-autoformat
" noremap <F3> :Autoformat<CR>

" NCM2
augroup NCM2
	autocmd!
	" Enable ncm2 for all buffers
	autocmd BufEnter * call ncm2#enable_for_buffer()
	" :help Ncm2PopupOpen for more information
	set completeopt=noinsert,menuone,noselect
	" When the <Enter> key is pressed while the popup menu is visible, it only
	" hides the menu. Use this mapping to close the menu and also start a new line.
	inoremap <expr> <CR> (pumvisible() ? "\<c-y>\<cr>" : "\<CR>")
	
	" Vimtex for LaTeX
	autocmd Filetype tex call ncm2#register_source({
		\ 'name': 'vimtex',
		\ 'priority': 8,
		\ 'scope': ['tex'],
		\ 'mark': 'tex',
		\ 'word_pattern': '\w+',
		\ 'complete_pattern': g:vimtex#re#ncm2,
		\ 'on_complete': ['ncm2#on_complete#omni', 'vimtex#complete#omnifunc'],
	\ })
	
	" Cycle through completion list with tab/shift+tab
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

" UltiSnips
let g:UltiSnipsExpandTrigger = '<C-j>'
let g:UltiSnipsJumpForwardTrigger = '<C-j>'
let g:UltiSnipsJumpBackwardTrigger = '<C-shift-j>'

"Python
let g:python3_host_prog='python3'

let mapleader=","
set timeout timeoutlen=1500

" Copy to clipboard
noremap <Leader>y "+y
noremap <Leader>p "+p

" Use ctrl-backspace to delete the previous word in insert mode.
noremap! <C-BS> <C-w>
noremap! <C-h> <C-w>

" Run python scripts with f5
autocmd FileType python map <buffer> <F5> :w<CR>:exec '!python3' shellescape(@%, 1)<CR>
autocmd FileType python imap <buffer> <F5> <esc>:w<CR>:exec '!python3' shellescape(@%, 1)<CR>

" Run bash scripts with f5
autocmd FileType sh map <buffer> <F5> :w<CR>:!bash %<CR>
autocmd FileType sh imap <buffer> <F5> <esc>:w<CR>:!bash %<CR>

" Compile and run C programs with f5
autocmd FileType c map  <buffer> <F5>      :w<CR>:exec '!cd ' . shellescape(expand('%:p:h'), 1) . ' && make && ' . shellescape(expand('%:p:r'), 1)<CR>
autocmd FileType c imap <buffer> <F5> <esc>:w<CR>:exec '!cd ' . shellescape(expand('%:p:h'), 1) . ' && make && ' . shellescape(expand('%:p:r'), 1)<CR>


" Vimtex Plugin settings
let g:tex_flavor='latex'
let g:tex_conceal=''
set conceallevel=0
let g:vimtex_fold_manual=1
let g:vimtex_latexmk_continuous=1
let g:vimtex_compiler_progname='nvr'
let g:vimtex_view_method='general'
let g:vimtex_view_general_viewer='zathura'
let g:syntastic_tex_checkers=['lacheck']

" Forward Searching in Zathura DOESN'T WORK YET
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

" User defined scroll mode, where j and k are mapped to ctrl-y and ctrl-e
 nnoremap gs :call <sid>scrollMode()<cr>
let s:scrollModeRecurse = 0
let s:scrollModeCombos = {
	\   'j': 'normal ',
	\   'J': 'normal 5',
	\   'k': 'normal ',
	\   'K': 'normal 5',
	\   'g': 'normal gg',
	\   'G': 'normal G',
	\}
function! s:scrollMode()
	let s:scrollModeRecurse += 1
	call libmodal#Enter('SCROLLMODE' . s:scrollModeRecurse, s:scrollModeCombos)
	let s:scrollModeRecurse -= 1
endfunction
