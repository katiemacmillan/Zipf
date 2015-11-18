#ifndef __SORT_HPP__
#define __SORT_HPP__
/*
sort.hpp
This is intended to provide a O(N) sort for frequency table entries.
This algorithm is based on O(N) distribution counting and radix sorts.

Author: Alex Iverson
*/

#include <string>
#include "myPair.hpp"

typedef myPair<int, std::string> tableEntry;

void sortWords(tableEntry words[], int length, int maxStrLen); //O(N) sort for the table entries

#endif
