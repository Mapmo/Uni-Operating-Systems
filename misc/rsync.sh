#!/bin/bash

#used for backuping my file system to an external hard drive

sudo rsync -aAXv / --exclude={"/dev/*","/proc/*","/sys/*","/tmp/*","/run/*","/mnt/*","/media/*","/lost+found","/timeshift/*","/boot/efi/*","/home/martin/VirtualBox VMs/*","/home/martin/Downloads/*","/home/martin/.cache","/var/tmp/*","/home/martin/.mozilla"} .

