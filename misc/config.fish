function fish_prompt
	set_color yellow
	echo -n (whoami)
	set_color magenta 
	echo -n @
	echo -n (hostname)
	set_color green 
	echo -n (builtin pwd)
	set_color normal
	echo -n '$'
end

export EDITOR=/usr/bin/vim
export VISUAL=/usr/bin/vim

#own aliases
alias лс=ls
alias жим=vim
alias цд=cd
alias судо=sudo
alias цлеар=clear
alias я=q
alias пвд=pwd
alias reboot="sudo reboot"
alias poweroff="sudo poweroff"
alias ецхо=echo
alias ман=man
alias ls="ls --color"

#aliases for my keyboard lighting
alias kon="xset led on"
alias кон=kon
alias kof="xset led off"
alias коф=kof

#aliases for shortened commands
alias ups="sudo pacman -Suy"
alias neo="neofetch"

function pwd
	echo
	echo "Welcome to "(builtin pwd)":"
	echo
	ls 
	echo
end

function cd
	echo

	if test -z $argv 
		builtin	cd ~
	else
		builtin cd $argv
	end	
	
	pwd
end

function q
	pkill firefox  #usually only my web browser is active when I try to poweroff
	#pkill chrome

	sleep 0.2

	sudo poweroff

end

test -s ~/.alias && . ~/.alias || true

set PNAME (ps --pid=$PPID --no-header | awk '{print $(NF)}')

if test "$PNAME" != 'gdm-x-session' #for GNOME GUI with xserver, prevents from errors on boot
	cmatrix -C cyan -s
end
