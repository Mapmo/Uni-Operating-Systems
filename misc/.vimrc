" execute pathogen#infect()

filetype plugin on
filetype indent on

" Syntastic rules
set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0
let g:syntastic_aggregate_errors = 1


" bash
let g:syntastic_enable_bash_checker=['shellcheck']
" perl
let g:syntastic_enable_perl_checker = 1
let g:syntastic_perl_lib_path = [ "/usr/share/perl5", './lib', ]
let g:syntastic_perl_checkers = ['perl']
" python
let g:syntastic_python_checkers=['flake8', 'python']
let g:syntastic_python_flake8_post_args='--extend-ignore=E501,E128,E225,W605'
" yaml
let g:syntastic_yaml_checkers = ['yamllint']
" Use /.vim/syntax/groovy.vim for groovy syntax check
au BufNewFile,BufRead *.groovy  setf groovy


" Indentation rules
setlocal ts=4 sts=4 sw=4 noexpandtab

autocmd Filetype bash setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype c setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype cpp setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype coffee setlocal ts=2 sts=2 sw=2 expandtab
autocmd Filetype go setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype h setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype js setlocal ts=4 sts=4 sw=4 expandtab
autocmd Filetype ruby setlocal ts=2 sts=2 sw=2 expandtab
autocmd Filetype sh setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype pl setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype py setlocal ts=4 sts=4 sw=4 expandtab
autocmd Filetype spec setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype tf setlocal ts=2 sts=2 sw=2 expandtab
autocmd Filetype yaml setlocal ts=2 sts=2 sw=2 expandtab
autocmd Filetype yml setlocal ts=2 sts=2 sw=2 expandtab

" Enable syntax highlighting by default
syntax on

" Highlight trailing whitespaces
:highlight ExtraWhitespace ctermbg=red guibg=red
:match ExtraWhitespace /\s\+$/
" Add parentheses matching
" :hi MatchParen cterm=underline ctermbg=none ctermfg=darkyellow
:hi MatchParen cterm=bold ctermbg=magenta ctermfg=blue
