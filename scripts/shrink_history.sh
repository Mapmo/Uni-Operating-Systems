#!/bin/bash

tmp="$(mktemp)" || exit 1
hist="$(echo  ~/.bash_history)"

if [[ -e "$hist" ]]
then
	sed -r 's/.*type .*|.*man .*|.*ls .*|.*cd .*|.*лс .*|.*цд .*|.*history .*//i' "$hist" | sort -u > "$tmp"
	cat "$tmp" > "$hist"
	rm "$tmp"
else
	echo "issue finding $hist"
	rm "$tmp"
	exit 1
fi

