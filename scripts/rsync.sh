#!/bin/bash

#used for backuping my file system to an external hard drive

sudo rsync -aAXv --delete / --exclude={"/dev/*","/proc/*","/sys/*","/tmp/*","/run/*","/mnt/*","/media/*","/lost+found","/boot/efi","/timeshift","/home/mapmo/vms","/home/mapmo/Downloads/*","/home/mapmo/Videos/*","/home/mapmo/.cache/*","/home/mapmo/.local/share/Trash/*","/var/tmp/*"} /mnt
