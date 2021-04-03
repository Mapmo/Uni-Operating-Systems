export EDITOR=/usr/bin/vim
export VISUAL=/usr/bin/vim
export PATH="$PATH:~/.local/bin"
export HISTFILESIZE=20000

#cyrilic aliases
alias ецхо=echo
alias жим=vim
alias лл="ls -l --color=auto"
alias лс="ls --color=auto"
alias ман=man
alias судо=sudo
alias пвд=pwd
alias цат=cat
alias цд=cd
alias цлеар=clear
alias я=q
alias нео="neofetch"

#aliases for shortened commands
alias add="ssh-add"
alias free="free -h"
alias ga="git add"
alias gcam="git commit -am"
alias gcm="git commit -m"
alias gcma="git commit -am"
alias gd="git diff"
alias gs="git status"
alias hist="history | grep"
alias i3c="vim ~/.config/i3/config"
alias jst="python -m json.tool < " # cat a file in pretty-json format
alias ll="ls -l"
alias ls="ls --color=auto"
alias neo="neofetch"
alias reboot="sudo reboot"
alias poweroff="sudo poweroff"
alias ups="sudo apt update && sudo apt upgrade"
alias nvd="__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia" #runs a program with the nvdia card
alias clip="xclip -sel clip"

pwd () {
	echo
	echo "Welcome to $(command pwd):"
	echo
	ls
	echo
}

cd () {
	echo

	if [ -z "$*" ]; then
		command	cd ~
	else
		command cd "$*"
	fi
	
	pwd
}

locate () {
    /usr/bin/locate "$1" | grep -v timeshift
}

show () {
	if [ $# -ne 1 ]; then
		echo "Pass an argument please"
		return 1
	fi

	less /proc/$1/status
}

qw () {
	pkill firefox  #usually only my web browser is active when I try to poweroff
	sleep 0.2
	sudo poweroff
}

#Function that is used in the $PS1 variable to show me if I am on a git branch
get_branch () {
    git branch 2>/dev/null | grep '^*' | tr '*' ':' | tr -d ' '
}

export PS1="\
\$(if [[ \$? -eq 0 ]]; then printf \"\[\e[1;33m\]\u\[\e[1;35m\]@\h:\[\e[1;32m\]\w\[\e[0;33m\]\$(get_branch)\[\e[1;32m\]\"; else printf \"\[\e[1;33m\]\u\[\e[1;35m\]@\h:\[\e[1;32m\]\w\[\e[0;33m\]\$(get_branch)\[\e[0;31m\]\"; fi)>$ \[\e[m\]"


#The following code adds a cool output when starting a new session. It causes troubles to X server on boot so a check is needed to avoid the warning

PNAME=$(ps --pid=$PPID --no-header | awk '{print $(NF)}')
if [ ${PNAME} != 'gdm-x-session' ]; then #for GNOME DE
    neo 
fi
