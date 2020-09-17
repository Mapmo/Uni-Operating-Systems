"за проврка на кода
execute pathogen#infect()

set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*
" vimrc файловете кото ползваме + таболациите + чевен цвят за спеисове които не ОК:
filetype plugin on
filetype indent on
let g:syntastic_perl_lib_path = [ "/usr/share/perl5", './lib', ]
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0
let g:syntastic_yaml_checkers = ['yaml']
let g:syntastic_perl_checkers = ['perl']
let g:syntastic_python_checkers=['python', 'flake8']
let g:syntastic_enable_perl_checker = 1
let g:syntastic_enable_yaml_yamlxs_checker = 1
let g:syntastic_aggregate_errors = 1
" Use /.vim/syntax/groovy.vim for groovy syntax check
au BufNewFile,BufRead *.groovy  setf groovy

setlocal ts=4 sts=4 sw=4 noexpandtab
"Set indentation rules
autocmd Filetype bash setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype c setlocal ts=8 sts=8 sw=8 noexpandtab
autocmd Filetype coffee setlocal ts=2 sts=2 sw=2 expandtab
autocmd Filetype go setlocal ts=8 sts=8 sw=8 noexpandtab
autocmd Filetype h setlocal ts=8 sts=8 sw=8 noexpandtab
autocmd Filetype jade setlocal ts=4 sts=4 sw=4 expandtab
autocmd Filetype js setlocal ts=4 sts=4 sw=4 expandtab
autocmd Filetype ruby setlocal ts=2 sts=2 sw=2 expandtab
autocmd Filetype sh setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype pl setlocal ts=4 sts=4 sw=4 noexpandtab 
autocmd Filetype py setlocal ts=4 sts=4 sw=4 expandtab 
autocmd Filetype spec setlocal ts=4 sts=4 sw=4 noexpandtab
autocmd Filetype yaml setlocal ts=4 sts=4 sw=4 expandtab
autocmd Filetype yml setlocal ts=2 sts=2 sw=2 expandtab

"line enables syntax highlighting by default.
syntax on
