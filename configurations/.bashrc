# Sample .bashrc for SuSE Linux
# Copyright (c) SuSE GmbH Nuernberg

# There are 3 different types of shells in bash: the login shell, normal shell
# and interactive shell. Login shells read ~/.profile and interactive shells
# read ~/.bashrc; in our setup, /etc/profile sources ~/.bashrc - thus all
# settings made here will also take effect in a login shell.
#
# NOTE: It is recommended to make language settings in ~/.profile rather than
# here, since multilingual X sessions would not work properly if LANG is over-
# ridden in every subshell.

# Some applications read the EDITOR variable to determine your favourite text
# editor. So uncomment the line below and enter the editor of your choice :-)
export EDITOR=/usr/bin/vim
#export EDITOR=/usr/bin/mcedit

# For some news readers it makes sense to specify the NEWSSERVER variable here
#export NEWSSERVER=your.news.server

# If you want to use a Palm device with Linux, uncomment the two lines below.
# For some (older) Palm Pilots, you might need to set a lower baud rate
# e.g. 57600 or 38400; lowest is 9600 (very slow!)
#
#export PILOTPORT=/dev/pilot
#export PILOTRATE=115200

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

alias ups="sudo apt update && sudo apt full-upgrade"

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
	command cd "$*"
	pwd
}

function q
{
	pkill chrome #usually only my chrome is active when I try to poweroff

	sleep 0.2

	sudo poweroff

}

test -s ~/.alias && . ~/.alias || true
export PS1="\[\e[1;33m\]\u\[\e[1;35m\]@\h:\[\e[1;32m\]\w>$ \[\e[m\]"

PNAME=$(ps --pid=$PPID --no-header | awk '{print $(NF)}')

if [ $PNAME != gdm-x-session ] #for GNOME GUI
then
	cmatrix -C cyan -s
fi
