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

4. The command chroot starts a new session where the current directory becomes root ( / ). However, there are required files for the new / to be usable, for instance /bin/bash . Users may need other programs as well.\
Write a bash script that copies all the required files for chrooting (/bin/bash is mandatory). User should be able to specify more required files or even required directories. User should also be able to choose  the chroot directory, if none is selected then assume it's the current working directory.\
Hint: use ldd to find the dependencies of a file and remember that these dependencies may also have dependencies

5. Create a shell script that will tell you all the files that are currently opened on the machine and it will give the output in the following format:\
  command0 || file0\
  command0 || file1\
  command0 || file2\
  command1 || file0

6. Write a bash script which will parse a text file with the following format:\
64.31.49.26|2013-07-24-06:51:01|Blocked by pesho\
108.16|2013-07-24-08:17:01|Blocked by niki\
183.12013-07-24-11:51:00|Blocked by mm\
183.29|2013-07-24-11:51:01|Blocked by joro\
112.16|2013-07-24-13:00:00|Blocked by toni\
220.14|2013-07-24-13:00:01|Blocked by pesho\
82.5013-07-24-13:51:00|Blocked by pesho\
103.62013-07-24-16:51:00|Blocked by misho\
111.2|2013-07-24-18:34:00|Blocked by lubo\
209.22013-07-24-18:34:00|Blocked by vlado\
201.1|2013-07-24-18:51:01|Blocked by joro\
79.19.133.36|2013-07-24-20:00:00|Blocked by toni\

The script has to output only the lines that have dates older then 14 days. Sort the output by date. 

7. Write a script that takes a the name of a file, that consists of numbers on separate lines, as a single paramter. The script should return the average of the numbers with 3 digits precision after the decimal point.

Example:

1\
2\
9\
8

=> 5.000

8. We provide you with expressions containing +,-,*,^, / and parenthesis. None of the numbers in the expression involved will exceed 999.Your task is to evaluate the expression and display the correct output rounding up to 3 decimal places. 

Examples:
5+50*3/20 + (19*2)/7

=> 17.929

-----------

-105+50*3/20 + (19^2)/7

=> -45.929

9. Given three integers representing the three sides of a triangle, identify whether the triangle is Scalene, Isosceles, or Equilateral.

Example:

2\
3\
4

=> SCALENE

10. Given multiple lines stream with the following format: 

NAME SCORE1 SCORE2 SCORE3

transform the line to the following format:

NAME SCORE1 SCORE2 SCORE3 : MARK

where mark is "FAIL" if the average score is < 50, B if the average score is < 80 and A otherwise.

Sample input:

A 25 27 50\
B 35 37 75\
C 75 78 80\
D 99 88 76

Sample output:

A 25 27 50 : FAIL\
B 35 37 75 : FAIL\
C 75 78 80 : B\
D 99 88 76 : A
