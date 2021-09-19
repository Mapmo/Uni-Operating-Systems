#!/bin/sh

if [ $(id -u) -ne 0 ]; then
	echo 'Root privileges needed'
	exit 1
fi	

sed 's/doublelowquotemark/less/' /usr/share/X11/xkb/symbols/bg -i
sed 's/leftdoublequotemark/greater/' /usr/share/X11/xkb/symbols/bg -i
