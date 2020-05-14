1. Print a single line containing all of the tag names found in the input. Your output tags should be semicolon-separated and ordered lexicographically (i.e.: alphabetically). Do not print the same tag name more than once.

Example: 

<p><a href="http://www.quackit.com/html/tutorial/html_links.cfm">Example Link</a></p>
<div class="more-info"><a href="http://www.quackit.com/html/examples/html_links_examples.cfm">More Link Examples...</a></div>

=> a;div;p

2. Write a regex for the following string:

	The string must begin with between 0-3 (inclusive) lowercase letters.\
    	Immediately following the letters, there must be a sequence of digits (0-9). The length of this segment must be between 2 and 8, both inclusive.\
    	Immediately following the numbers, there must be atleast 3 uppercase letters.

Your task is to find out if a given identification number is valid or not.\
For every identification number, print whether it is VALID or INVALID

Sample Input:\
abc012333ABCDEEEE\
0123AB

Sample Output:\
VALID\
INVALID

3. Strip the links and the text name from the html pages. A html link is of the form:

<a href="http://www.example.com">Text</a>\
<a href="http://www.example.com"><h1><b>Text</b></h1></a>

Sample Input:
<a href="http://www.example.com"><h1><b>Text</b></h1></a>
<a href="http://www.example.com"><area><b>Text</b><area></a>
<a href="http://www.example.com"><i hreflang="see"><b>Text</b></i></a>
<a href="http://www.example.com"></a>

Sample Output:
http://www.example.com,Text
http://www.example.com,Text
http://www.example.com,Text
http://www.example.com,

4. Write a regex to extract all emails in a text, then sort them and present them on a single line separated by ';'. A valid email begins and ends with a letter or a digit, has to contain '@' and at least one '.' after it and cannot contain non-ASCII characters or white spsaces.

Sample Input:
Product Hacker product@hackerrank.com
Mail us at hackers@hackerrank.com™ to chat more. Or you can write to us at interviewstreet@hackerrank.com!

Sample Output:
hackers@hackerrank.com;interviewstreet@hackerrank.com;product@hackerrank.com

5. The following regex is supposed to match valid email addresses

	egrep -o [^[:blank:]]+@[^[:blank:]]+\.[^[:blank:]]+[[:alnum:]]

When tested, however:

	echo "shame@example.org hahaha" | egrep -o [^[:blank:]]+@[^[:blank:]]+\.[^[:blank:]]+[[:alnum:]]

	>shame@example.org hahaha

Find out the reason why this is happening

6. We define a word character to be any of the following:\
	An English alphabetic letter (i.e., a-z and A-Z).\
	A decimal digit (i.e., 0-9).\
	An underscore

We define a subword to be a word strictly inside another word (e.g. "is" in "exISisting, permISsive, etc.) but not in the end of a word (e.g. INteresting, skIN)

Given N sentences consisting of one or more words separated by non-word characters, process Q queries where each query consists of a single string, S. To process each query, count the number of occurrences of S as a sub-word in all N sentences, then print the number of occurrences on a new line. 

Sample Input
1
existing pessimist optimist this is
1
is

Sample Output
3

7. Write a regex which will match the following string S

	S consists of 8 digits.\
	may have "-" separator such that string gets divided in parts, with each part having exactly two digits

	Valid S\
	12345678\
	12-34-56-87

	Invalid S\
	1-234-56-78\
	12-45-7810

8. Write a regex which matches the string S

	S consists of tic or tac.\
	tic should not be immediate neighbour of itself.\
	The first tic must occur only when tac has appeared at least twice before.

	Valid\
	tactactic\
	tactactictactic

	Invalid\
	tactactictactictictac\
	tactictac
