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
{
	echo
	echo "Welcome to $(command pwd):"
	echo
	ls 
	echo
}

function cd
{
	echo

	if [ -z "$*" ]
	then
		command	cd ~
	else
		command cd "$*"
	
	fi
	
	pwd
}

function q
{
	pkill firefox  #usually only my web browser is active when I try to poweroff
	#pkill chrome

	sleep 0.2

	sudo poweroff

}

test -s ~/.alias && . ~/.alias || true
export PS1="\[\e[1;33m\]\u\[\e[1;35m\]@\h:\[\e[1;32m\]\w>$ \[\e[m\]"

PNAME=$(ps --pid=$PPID --no-header | awk '{print $(NF)}')

if [ ${PNAME} != 'gdm-x-session' ] #for GNOME GUI
then
	cmatrix -C cyan -s
fi
