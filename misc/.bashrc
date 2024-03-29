export EDITOR=/usr/bin/vim
export VISUAL=/usr/bin/vim
export PATH="$PATH:~/.local/bin"
export HISTFILESIZE=20000
export HISTSIZE=20000

#cyrilic aliases
alias ецхо=echo
alias жим=vim
alias лл="ls -l --color=auto"
alias лс="ls --color=auto"
alias ман=man
alias нео="neofetch"
alias судо=sudo
alias пвд=pwd
alias цат=cat
alias цд=cd
alias цлеар=clear
alias яв=qw

#aliases for shortened commands
alias add="ssh-add"
alias free="free -h"
alias ga="git add"
alias gb="git branch"
alias gc="git checkout"
alias gcb="git checkout -b"
alias gcam="git commit -am"
alias gcm="git commit -m"
alias gcma="git commit -am"
alias gd="git diff"
alias gs="git status"
alias hist="history | grep -v hist | grep"
alias i3c="vim ~/.config/i3/config"
alias jst="python3 -m json.tool < " # cat a file in pretty-json format
alias k8s="kubectl"
alias mk="minikube"
alias mkup="minikube start --driver=docker"
alias lol="lolcat"
alias ll="ls -l"
alias ls="ls --color=auto"
alias neo="neofetch | lol"
alias tf="terraform"
alias reboot="sudo reboot"
alias poweroff="sudo poweroff"
alias ups="sudo dnf upgrade -y"
alias clip="xclip -sel clip"

pwd () {
	echo
	echo "Welcome to $(command pwd):"
	echo
	ls
	echo
}

cd () {
	if [ -z "$*" ]; then
		command	cd ~
	else
		command cd "$*"
	fi

	pwd
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
	poweroff
}

# === PS1 setup ====
# Function that is used in the $PS1 variable to show me if I am on a git branch
get_branch () {
	gs -s &>/dev/null || return
	echo -n ':'
	git branch --show-current 2>/dev/null
}

export PS1='$(tmp=$?; if [ $tmp -eq 1 ]; then color="0;31"; else color="1;32"; fi; printf "\[\e[1;33m\]\u\[\e[1;35m\]@\h:\[\e[1;32m\]\w\[\e[0;33m\]$(get_branch)\[\e[${color}m\]")>$ \[\e[m\]'

# ==================

# The following code adds a cool output when starting a new session.
# It causes troubles to X server on boot so a check is needed to avoid the warning
PNAME=$(ps --pid=$PPID --no-header | awk '{print $(NF)}')
if [ ${PNAME} != 'gdm-x-session' ]; then
	neo 2>/dev/null
fi
