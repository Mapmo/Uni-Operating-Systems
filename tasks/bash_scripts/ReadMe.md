1. You have a named configuration file with the follwoing content:

zone "zoo.net" {\
       type master;\
       file "/var/www/normal-zones/zoo.net";\
       allow-transfer { 44.15.123.171; 14.253.73.174; };\
};\
zone "doomla.bg" {\
       type master;\
       file "/var/named/normal-zones/doomla.bg";\
};\
zone "sblg.com" {\
       type slave;\
       file "/var/named/sblg.com.";\
       //masters { 7.24.12.30; 18.22.28.131; };\
       masters { uhu-gcp; };\
};

   a) Write a bash script that takes 2 parameters - name of a file and name of a zone to delete and deletes the given zone record if present.\
   b) Write a bash script that takes 2 parameters - name of a file and a series of ips and edits the IPs(you don't need to know what the previous IPs were) for allow-transfer in all records.


2. Write a bash script that will create a backup of your home folder, every 1h.

    the script should make sure that it will keep only 10 backups.\
    the 11th(oldest) backup should be removed, every time.
    there should be only one instance of the script running.\

3. You have a log file with multiple records in the following format

log-02-05-2020 {\
	successful-builds: 5432;\
	failed-builds: 31;\
}

	a) Write a bash script that validates that there is no record where (successful-builds - failed-builds > 2000)
	b) Write a bash script that prints all records where failed-builds are more than or equal to successful builds
