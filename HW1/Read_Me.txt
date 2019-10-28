David Davis
dwd2@pdx.edu
PSU Id: 978105326

Homework 1 Dinner Problem

I wrote my program in C++ on a Linux Ubuntu 18.04 running with 4-Core 4GB Ram Dell XPS. 

The program is run with the target text file name as the first command line argument.

The program starts by determining the net likeness for each possible seating pair and sorts the list from least (lowest net score) to best. I then assign every odd location at the dinner table with the list of people in the group of least like pairs. My plan was to avoid the points of a possible pairing resulting from the least liked possible pairs. I then planned to assign each of the remaining people, one at a time, to where they fit the best out of the remaing seats. I ran out of time and was only able to implement a complete seach. 

The results from the current implementation are far below expected pointing to a bad heuristic of removing the worst possible pairings. I thought that they would be benificial, but will need to 
revaluate my decisions.



