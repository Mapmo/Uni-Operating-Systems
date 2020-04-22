function fish_prompt
set_color yellow
    echo -n (whoami)
    set_color magenta 
    echo -n @
    echo -n (hostname)
    set_color green 
    echo -n (pwd)
    set_color normal
    echo -n ' > '
end
