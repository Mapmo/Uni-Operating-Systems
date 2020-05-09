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
