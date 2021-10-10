#!/usr/bin/env bash

if [ $(id -u) -ne 0 ]; then
	echo "Root permission required"
	exit 1
fi

apt_packages=(
	brightnessctl
	figlet
	fonts-noto-color-emoji
	imagemagick
	i3
	i3-wm
	i3lock
	i3status
	lolcat
	lxappearance
	scrot
	vim
	vim-syntastic
	xclip
	xinit
	xorg
)

pip3_packages=(
	grip
	i3ipc
	flake8
	yamllint
)

apt install "${apt_packages[@]}" -y
pip3 install "${pip3_packages[@]}"
