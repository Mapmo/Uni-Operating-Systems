export EDITOR=/usr/bin/vim
export VISUAL=/usr/bin/vim

#cyrilic aliases
alias ецхо=echo
alias жим=vim
alias лл="ls -l --color=auto"
alias лс="ls --color=auto"
alias ман=man
alias судо=sudo
alias пвд=pwd
alias цд=cd
alias цлеар=clear
alias я=q

#aliases for my keyboard lighting
alias kon="xset led on"
alias кон=kon
alias kof="xset led off"
alias коф=kof

#aliases for shortened commands
alias ll="ls -l --color=auto"
alias ls="ls --color=auto"
alias neo="neofetch"
alias reboot="sudo reboot"
alias poweroff="sudo poweroff"
alias ups="sudo pacman -Syu"

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

function locate
{
    /usr/bin/locate $1 | grep -v timeshift
}

export PS1="\[\e[1;33m\]\u\[\e[1;35m\]@\h:\[\e[1;32m\]\w>$ \[\e[m\]"


#The following code adds a cool output when starting a new session. It causes troubles to X server on boot so a check is needed to avoid the warning

PNAME=$(ps --pid=$PPID --no-header | awk '{print $(NF)}')
if [ ${PNAME} != 'gdm-x-session' ] #for GNOME DE
then
	cmatrix -C cyan -s
fi
