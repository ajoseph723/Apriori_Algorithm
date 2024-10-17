# Apriori Algorithm
Algorithm that takes a file with lists of transactions and finds what items in the lists are commonly together aka finding frequent itemsets.

# Usage
First compile the Apriori.cpp file with:
```
g++ [file path to Apriori.cpp]
```
Next run the compiled code with the file that contains the list and the minimum support level (should be a number between 0-100) that will determine whether an itemset will be frequent or not.
```
./[file with compiled code] [list file] [minimum support level]
