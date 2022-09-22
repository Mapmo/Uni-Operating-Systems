#!/usr/bin/env bash

if [ $(id -u) -ne 0 ]; then
	echo "Root permission required"
	exit 1
fi

apt_packages=(
	brightnessctl
	feh
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
	xset
)

dnf_packages=(
	brightnessctl
	dmenu
	feh
	figlet
	flatpak
	google-noto-emoji-fonts
	ImageMagick
	i3
	i3lock
	i3status
	lolcat
	lxappearance
	scrot
	vim
	vim-syntastic
	xclip
	xinit
	xset
	xfce4-screenshooter
)

pip3_packages=(
	grip
	i3ipc
	flake8
	yamllint
)

flatpak_packages=(
	 com.discordapp.Discord
	 com.visualstudio.code
	 us.zoom.Zoom
 )

type apt 2>/dev/null && apt install "${apt_packages[@]}" -y
type dnf 2>/dev/null && dnf install "${dnf_packages[@]}" -y

pip3 install "${pip3_packages[@]}"

flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub "${flatpak_packages[@]}" -y
